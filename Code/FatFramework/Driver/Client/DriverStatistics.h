#pragma once

#include "FatFramework/Kernel/Common/Types.h"
#include <sstream>

namespace Fat {

class IDriverStatistics
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	// Format statistics for display
	virtual void Format(std::wostringstream& out) = 0;

	// Reset counter
	virtual void Reset() = 0;

	virtual void SetClientFps(SInt32 value) = 0;
	virtual void SetServerFps(SInt32 value) = 0;
	virtual void SetGpuFps(SInt32 value) = 0;

	virtual void SetCommandBufferSize(SInt32 value) = 0;
	virtual void IncBuiltCommandsPerFrame() = 0;
	virtual void IncDispatchedCommandsPerFrame() = 0;
	virtual void IncD3D9CallsPerFrame() = 0;
};
extern IDriverStatistics *GDriverStats;

}
