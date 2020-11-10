#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/PlatformHeaders.h"
#include "FatFramework/Kernel/Common/Types.h"
#include "FatFramework/Kernel/Common/NonCopyable.h"

#if !FAT_OS_WINDOWS
#include "FatFramework/Kernel/Thread/Mutex.h"
#include "FatFramework/Kernel/Thread/ConditionVariable.h"
#endif

namespace Fat {

class Event : private NonCopyable
{
public:
	Event();
	~Event();

	void Set();
	void Wait() const;
	bool Wait(UInt32 timeoutMillis) const;

private:
#if FAT_OS_WINDOWS
	HANDLE handle_;
#else
	Mutex lockNotify_;
	ConditionVariable cond_;
	volatile bool flag_;
#endif
};

}
