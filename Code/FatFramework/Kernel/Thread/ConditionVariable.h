#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/PlatformHeaders.h"
#include "FatFramework/Kernel/Common/Types.h"
#include "FatFramework/Kernel/Common/NonCopyable.h"
#include "FatFramework/Kernel/Thread/Mutex.h"
#include "FatFramework/Kernel/Thread/MutexFast.h"

namespace Fat {

class ConditionVariable : private NonCopyable
{
public:
	ConditionVariable();
	~ConditionVariable();

	void Wait(Mutex& lock);
	void Wait(MutexFast& lock);
	bool TimedWait(Mutex& lock, UInt32 millis);
	bool TimedWait(MutexFast& lock, UInt32 millis);
	void NotifyOne();
	void NotifyAll();

private:
#if FAT_OS_WINDOWS
	CONDITION_VARIABLE cond_;
#else
	pthread_cond_t cond_;
#endif
};

}
