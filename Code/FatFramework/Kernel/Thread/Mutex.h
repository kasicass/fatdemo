#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/PlatformHeaders.h"
#include "FatFramework/Kernel/Common/Types.h"
#include "FatFramework/Kernel/Thread/TAutoLock.h"

namespace Fat {

// recursive mutex
class Mutex
{
	friend class ConditionVariable;

public:
	Mutex();
	~Mutex();

	void Lock();
	void Unlock();
	bool TryLock();

private:
	Mutex(const Mutex& rhs);
	Mutex& operator=(const Mutex& rhs);

private:
#if defined(FAT_OS_WINDOWS)
	SRWLOCK srwlock_;
	UInt32 recurseCounter_;
	ThreadId exclusiveOwningThreadId_;
#else
	pthread_mutex_t mutex_;
#endif
};

}

typedef Fat::TAutoLock<Fat::Mutex> MutexLocker;
