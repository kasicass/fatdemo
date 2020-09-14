#include "FatFramework.h"

namespace Fat {

//
// Packet
//

Packet::Packet()
{
	FatAssertUnreachableCode();
}

Packet::Packet(const ReadOnlyLockerHolder& lockerHolder) :
	ReadOnlyLockerHolder(lockerHolder)
{
}

Packet::Packet(const ICommand* pCommand) :
	ReadOnlyLockerHolder(pCommand)
{
}

Packet::Packet(const Packet& rhs) :
	ReadOnlyLockerHolder(rhs)
{
}

Packet::~Packet()
{
}

void Packet::Dispatch(Device& device)
{
	IServerDevice* pServerDevice = device.GetServerDevice();
	IServerCommand* pServerCommand = GetServerCommand();

	const ICommandData* pCommandData = GetData();
	if (pCommandData->IsRebuildNeeded())
	{
		pServerCommand->Build(pCommandData, pServerDevice);
		pCommandData->Rebuilt();
		theDriverStats->IncBuiltCommandsPerFrame();
	}

	pServerCommand->Dispatch(pCommandData, pServerDevice);
	theDriverStats->IncDispatchedCommandsPerFrame();
}

//
// Buffer
//

Buffer::Buffer()
{
}

Buffer::~Buffer()
{
	FatAssertNoText(packets_.empty());
}

void Buffer::QueuePacket(const Packet& packet)
{
	packets_.push_back(packet);
}

void Buffer::Dispatch(Device& device)
{
	for (auto it = packets_.begin(); it != packets_.end(); ++it)
	{
		it->Dispatch(device);
	}

	theDriverStats->SetCommandBufferSize(Int32(packets_.size() * sizeof(Packet)));
	Clear();
}

void Buffer::Clear()
{
	packets_.clear();
}

Bool Buffer::IsEmpty()
{
	return packets_.empty();
}

//
// CommandBuffer
//

CommandBuffer::CommandBuffer(Device& device, EBufferingMode::EValue mode) :
	device_(device),
	mode_(mode),
	insideBeginEnd_(false),
	exitServerThread_(false),
	buffers_(NULL),
	bufferCount_(0),
	clientFpsCounter_(0.5f),
	serverFpsCounter_(0.5f)
{
	pServerThread_ = Thread::Create("Server", &CommandBuffer::ServerEntryFunction, this);
	InitializeMode(mode);
}

CommandBuffer::~CommandBuffer()
{
	FatAssert(insideBeginEnd_ == false, L"Can not be called inside a begin/end pair");

	WaitAllBuffersFlushed();

	{
		MutexFastLocker locker(queuingMutex_);
		exitServerThread_ = true;
		queuingCondition_.NotifyOne();
	}

	pServerThread_->Join();
	DeallocateBuffers();
}

Bool CommandBuffer::Begin()
{
	FatAssert(insideBeginEnd_ == false, L"Already inside a begin/end pair");

	if (device_.IsResetNeeded())
	{
		WaitAllBuffersFlushed();

		Bool reseted = device_.Reset();
		if (!reseted)
			return false; // reset fails
	}

	insideBeginEnd_ = true;
	if (mode_ == EBufferingMode::eNoBuffering)
		return true;

	// In asynchronous mode, ensures a buffer is ready to be filled

	MutexFastLocker locker(queuingMutex_);

	switch (mode_)
	{
	case EBufferingMode::eDoubleBuffer:
		// Waits for a buffer to be available
		// Buffer will be released by the server and pushed into the client list
		while (clientBuffers_.empty())
			queuingCondition_.Wait(queuingMutex_);
		break;

	case EBufferingMode::eTripleBuffer:
		if (clientBuffers_.empty())
		{
			// Discards the oldest buffer sent to the server

			Buffer* pBuffer = serverBuffers_.back();
			serverBuffers_.pop_back();
			pBuffer->Clear();
			clientBuffers_.push_front(pBuffer);
		}
		break;

	default:
		FatAssertUnreachableCode();
		break;
	}

	FatAssertNoText(GetBufferToFill()->IsEmpty());
	return true;
}

void CommandBuffer::End()
{
	FatAssert(insideBeginEnd_ == true, L"Not inside a begin/end pair");

	insideBeginEnd_ = false;
	clientFpsCounter_.Update();
	theDriverStats->SetClientFps(clientFpsCounter_.GetFramePerSecond());

	if (mode_ == EBufferingMode::eNoBuffering)
		return;

	// In asynchronous mode, forwards the buffer that was just filled to the server
	{
		MutexFastLocker locker(queuingMutex_);

		// Remove buffer from the client list. Last filled buffer was at the front of the list
		Buffer* pBuffer = clientBuffers_.front();
		clientBuffers_.pop_front();

		// Give it to the server. Push it in the front as it is the newest one (the oldest is at the back)
		serverBuffers_.push_back(pBuffer);

		// Notify the server that it has a new buffer to flush
		queuingCondition_.NotifyOne();
	}
}

void CommandBuffer::SetBufferingMode(EBufferingMode::EValue mode)
{
	FatAssert(insideBeginEnd_ == false, L"Cannot be called inside a begin/end pair");

	if (mode_ == mode)
		return;

	WaitAllBuffersFlushed();
	mode_ = mode;
	InitializeMode(mode);
}

EBufferingMode::EValue CommandBuffer::GetBufferingMode() const
{
	return mode_;
}

void CommandBuffer::WaitAllBuffersFlushed()
{
	MutexFastLocker locker(queuingMutex_);

	while (clientBuffers_.size() != bufferCount_)
	{
		queuingCondition_.Wait(queuingMutex_);
	}
}

void CommandBuffer::Queue(const ICommand* pCommand)
{
	FatAssert(insideBeginEnd_ == true, L"Can only called inside a begin/end pair");

	Packet packet(pCommand);
	_Queue(packet);
}

void CommandBuffer::Queue(const ReadOnlyLockerHolder& lockerHolder)
{
	FatAssert(insideBeginEnd_ == true, L"Can only called inside a begin/end pair");

	Packet packet(lockerHolder);
	_Queue(packet);
}

void CommandBuffer::_Queue(Packet& packet)
{
	if (mode_ == EBufferingMode::eNoBuffering)
	{
		packet.Dispatch(device_);
	}
	else
	{
		Buffer* pBuffer = GetBufferToFill();
		pBuffer->QueuePacket(packet);
	}
}

void CommandBuffer::ServerEntryFunction(void* args)
{
	CommandBuffer* pCommandBuffer = (CommandBuffer*)args;
	pCommandBuffer->ServerFlushFunction();
}

void CommandBuffer::ServerFlushFunction()
{
	while (1)
	{
		queuingMutex_.Lock();
		while (!exitServerThread_ && serverBuffers_.empty())
		{
			queuingCondition_.Wait(queuingMutex_);
		}

		if (exitServerThread_)
		{
			queuingMutex_.Unlock();
			break;
		}

		Buffer* pBuffer = PickBufferToFlush(queuingMutex_);

		// Unlock mutex during server execution of buffer
		// Indeed, command buffer dispatching runs in parallel
		queuingMutex_.Unlock();
		pBuffer->Dispatch(device_);
		queuingMutex_.Lock();

		// Pushes pBuffer at the back of the list, as the front is used for the filled buffer
		// then notify a buffer was release to the client
		clientBuffers_.push_back(pBuffer);
		queuingCondition_.NotifyOne();

		// Compute fps between two Dispatch() calls
		serverFpsCounter_.Update();
		theDriverStats->SetServerFps(serverFpsCounter_.GetFramePerSecond());

		queuingMutex_.Unlock();
	}

	// Clear all server buffers before exiting
	MutexFastLocker locker(queuingMutex_);
	DiscardAllServerBuffers(queuingMutex_);
}

void CommandBuffer::DeallocateBuffers()
{
	FatAssert(insideBeginEnd_ == false, L"Cannot be called inside a begin/end pair");

	FatAssert(clientBuffers_.size() == bufferCount_ && serverBuffers_.size() == 0,
		L"All buffers must have been flushed prior to calling this function");

	clientBuffers_.clear();
	FatDeleteArray(buffers_);
}

void CommandBuffer::InitializeMode(EBufferingMode::EValue mode)
{
	DeallocateBuffers();

	bufferCount_ = 0;
	switch (mode)
	{
	case EBufferingMode::eNoBuffering:
		bufferCount_ = 0;
		break;

	case EBufferingMode::eDoubleBuffer:
		bufferCount_ = 2;
		break;

	case EBufferingMode::eTripleBuffer:
		bufferCount_ = 3;
		break;

	default:
		FatAssertUnreachableCode();
		break;
	}

	buffers_ = FatNewArray(Buffer, bufferCount_);
	for (UInt32 i = 0; i < bufferCount_; ++i)
	{
		clientBuffers_.push_back(&buffers_[i]);
	}

	// Reset driver stats
	theDriverStats->Reset();
}

Buffer* CommandBuffer::PickBufferToFlush(MutexFast& mutex)
{
	FatAssertNoText(clientBuffers_.size() + serverBuffers_.size() == bufferCount_);

	Buffer* pBuffer = NULL;
	switch (mode_)
	{
	case EBufferingMode::eDoubleBuffer:
		// Picks up the buffer to flush which is at the back of the list
		// It's very unlikely that the other buffer is already ready to flushed
		pBuffer = serverBuffers_.back();
		serverBuffers_.pop_back();
		break;

	case EBufferingMode::eTripleBuffer:
		// Picks up the newest buffer and discard oldests
		// New buffers are pushed at front, so the oldest is at the back
		pBuffer = serverBuffers_.front();
		serverBuffers_.pop_front();
		DiscardAllServerBuffers(mutex);
		break;

	default:
		FatAssertUnreachableCode();
		break;
	}

	FatAssertNoText(pBuffer != NULL);
	return pBuffer;
}

Buffer* CommandBuffer::GetBufferToFill()
{
	FatAssert(insideBeginEnd_ == true, L"Can only called inside a begin/end pair");
	return clientBuffers_.front();
}

void CommandBuffer::DiscardAllServerBuffers(MutexFast& mutex)
{
	while (!serverBuffers_.empty())
	{
		Buffer* pBuffer = serverBuffers_.back();
		serverBuffers_.pop_back();

		mutex.Unlock();
		pBuffer->Clear();
		mutex.Lock();

		clientBuffers_.push_back(pBuffer);
	}
}

}
