#include "FatFramework/Driver/Client/DriverStatistics.h"
#include "FatFramework/Kernel/Common/Log.h"
#include "FatFramework/Kernel/Thread/AtomicInt.h"
#include <iostream>

namespace Fat {

class DriverStatistics : public IDriverStatistics
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	// Format statistics for display
	virtual void Format(std::wostringstream& out) override;

	// Reset counter
	virtual void Reset() override;

	virtual void SetClientFps(SInt32 value) override;
	virtual void SetServerFps(SInt32 value) override;
	virtual void SetGpuFps(SInt32 value) override;

	virtual void SetCommandBufferSize(SInt32 value) override;
	virtual void IncBuiltCommandsPerFrame() override;
	virtual void IncDispatchedCommandsPerFrame() override;
	virtual void IncD3D9CallsPerFrame() override;

private:
	AtomicInt clientFps_;
	SInt32 serverFps_;
	SInt32 gpuFps_;
	SInt32 commandBufferSize_;
	SInt32 builtCommandsPerFrame_;
	SInt32 dispatchedCommandsPerFrame_;
	SInt32 d3d9CallsPerFrames_;
};

static DriverStatistics myDriverStats;
IDriverStatistics *GDriverStats = &myDriverStats;


void DriverStatistics::Init()
{
	FatLog(L"<Driver>: DriverStats Init");
	Reset();
}

void DriverStatistics::Shutdown()
{
	FatLog(L"<Driver>: DriverStats Shutdown");
}

void DriverStatistics::Reset()
{
	clientFps_ = 0;
	serverFps_ = -1;
	gpuFps_    = -1;
	commandBufferSize_          = 0;
	builtCommandsPerFrame_      = 0;
	dispatchedCommandsPerFrame_ = 0;
	d3d9CallsPerFrames_         = 0;
}

void DriverStatistics::Format(std::wostringstream& out)
{
	out << "-----------------\n";
	out << "Driver statistics\n";

	// client fps
	out << "-Client thread fps: " << clientFps_ << "\n";

	// server fps
	out << "-Server thread fps: ";
	if (serverFps_ != -1)
	{
		out << serverFps_;
	}
	else
	{
		out << "No loop detected";
	}
	out << '\n';

	// gpu fps
	out << "-GPU fps: ";
	if (gpuFps_ != -1)
	{
		out << gpuFps_;
	}
	else
	{
		out << "Unavailable";
	}
	out << '\n';
	
	out << "-Command buffer size: " << commandBufferSize_ << " Bytes\n";

	out << "-Built driver commands: " << builtCommandsPerFrame_ << '\n';
	builtCommandsPerFrame_ = 0;

	out << "-Dispatched driver commands: " << dispatchedCommandsPerFrame_ << '\n';
	dispatchedCommandsPerFrame_ = 0;

	out << "-D3D9 calls: " << d3d9CallsPerFrames_ << '\n';
	d3d9CallsPerFrames_ = 0;
}

void DriverStatistics::SetClientFps(SInt32 value)
{
	clientFps_ = value;
}

void DriverStatistics::SetServerFps(SInt32 value)
{
	serverFps_ = value;
}

void DriverStatistics::SetGpuFps(SInt32 value)
{
	gpuFps_ = value;
}

void DriverStatistics::SetCommandBufferSize(SInt32 value)
{
	commandBufferSize_ = value;
}

void DriverStatistics::IncBuiltCommandsPerFrame()
{
	++builtCommandsPerFrame_;
}

void DriverStatistics::IncDispatchedCommandsPerFrame()
{
	++dispatchedCommandsPerFrame_;
}

void DriverStatistics::IncD3D9CallsPerFrame()
{
	++d3d9CallsPerFrames_;
}

}
