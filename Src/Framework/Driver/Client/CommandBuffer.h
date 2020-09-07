#pragma once

namespace Fat {

class Device;

// Implement a command buffer packet, containing the data and the server object to dispatch.
// Implementation is based on the ReadOnlyLockerHolder object.
class Packet : public ReadOnlyLockerHolder
{
public:
	// Construct a packet from a server command and a data already packed into a Lock holder objects. It is
	// used for state queuing as the state object is stored in this two object when it is bound to the cache.
	explicit Packet(const ReadOnlyLockerHolder& lockerHolder);

	explicit Packet(const ICommand* pCommand);

	explicit Packet(const Packet& rhs);

	~Packet();

	void Dispatch(Device& device);

private:
	Packet();
};

// Implement a buffer of packets
class Buffer : public NonCopyable
{
public:
	Buffer();
	~Buffer();

	void QueuePacket(const Packet& packet);
	void Dispatch(Device& device);

	// Clear all the packets. It releases every references and acquisitions over the server commands and data
	void Clear();

	Bool IsEmpty();

private:
	typedef std::vector<Packet> PacketVector;
	PacketVector packets_;
};

FAT_ENUM_BEGIN(EBufferingMode)
	eNoBuffering,
	eDoubleBuffer,
	eTripleBuffer,
FAT_ENUM_END()

class CommandBuffer
{
public:
	CommandBuffer(Device& device, EBufferingMode::EValue mode = EBufferingMode::eTripleBuffer);
	virtual ~CommandBuffer();

	// Notify command buffer queuing is beginning. Queuing must be ended before calling this function.
	// Return true if begin succeed, otherwise it fails and no queuing could be donw. Also End() should not
	// be called if Begin() has failed.
	virtual Bool Begin();

	// Notify command buffer queuing is ending. Queuing must be began before calling this function.
	virtual void End();

	// Modify command buffer execution mode. Must be called outside of the begin end frame as it will wait
	// for all frames to be flushed.
	void SetBufferMode(EBufferingMode::EValue mode);

	EBufferingMode::EValue GetBufferingMode() const;

	void WaitAllBuffersFlushed();

protected:
	void Queue(const ICommand* pCommand);
	void Queue(const ReadOnlyLockerHolder& lockerHolder);

private:
	// Static server thread entry function, forwards call to ServerFlushFunction.
	static void ServerEntryFunction(void* args);

	// In charge of going through the command buffer, building and dispatching server commands.
	void ServerFlushFunction();

private:
	void _Queue(Packet& packet);

	void DeallocateBuffers();

	void InitializeMode(EBufferingMode::EValue mode);

	Buffer* PickBufferToFlush(MutexFast& mutex);
	Buffer* GetBufferToFill();

	void DiscardAllServerBuffers(MutexFast& mutex);

private:
	Device& device_;
	EBufferingMode::EValue mode_;

	Bool insideBeginEnd_;
	ThreadPtr pServerThread_;
	MutexFast queuingMutex_;
	ConditionVariable queuingCondition_;
	Bool exitServerThread_;

	Buffer* buffers_;
	UInt32 bufferCount_;

	// Collection of buffers to be flushed on the server side. The newest buffer is at the front and
	// oldest at the back.
	typedef std::list<Buffer*> ServerBuffers;
	ServerBuffers serverBuffers_;

	// Collection of buffers to be flushed on the client side. The newest buffer is at the front and
	// oldest at the back.
	typedef std::list<Buffer*> ClientBuffers;
	ClientBuffers clientBuffers_;

	FpsAutoCounter clientFpsCounter_;
	FpsAutoCounter serverFpsCounter_;
};

}
