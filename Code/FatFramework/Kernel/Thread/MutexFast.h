#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/PlatformHeaders.h"
#include "FatFramework/Kernel/Thread/TAutoLock.h"

namespace Fat {

// non-recursive mutex
class MutexFast
{
	friend class ConditionVariable;

public:
	MutexFast();
	~MutexFast();

	void Lock();
	void Unlock();
	bool TryLock();

private:
	MutexFast(const MutexFast& rhs);
	MutexFast& operator=(const MutexFast& rhs);

private:
#if defined(FAT_OS_WINDOWS)
	SRWLOCK srwlock_;
#else
	pthread_mutex_t mutex_;
#endif
};

}

typedef Fat::TAutoLock<Fat::MutexFast> MutexFastLocker;
