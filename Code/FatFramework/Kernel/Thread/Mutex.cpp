#include "FatFramework/Kernel/Thread/Mutex.h"
#include "FatFramework/Kernel/Thread/ThreadUtil.h"
#include "FatFramework/Kernel/Common/Assertion.h"

namespace Fat {

#if defined(FAT_OS_WINDOWS)

// SRW Lock (Slim Reader/Writer Lock)
// Faster + lighter than CriticalSection, and only enters into kernel mode if contented.

Mutex::Mutex() :
	srwlock_(SRWLOCK_INIT),
	recurseCounter_(0),
	exclusiveOwningThreadId_(THREADID_NULL)
{
}

Mutex::~Mutex()
{
}

void Mutex::Lock()
{
	const ThreadId threadId = ThreadUtil::CurrentThreadId();
	if (threadId == exclusiveOwningThreadId_)
	{
		++recurseCounter_;
	}
	else
	{
		AcquireSRWLockExclusive(&srwlock_);
		FatAssertNoText(recurseCounter_ == 0);
		FatAssertNoText(exclusiveOwningThreadId_ == THREADID_NULL);
		exclusiveOwningThreadId_ = threadId;
	}
}

void Mutex::Unlock()
{
	const ThreadId threadId = ThreadUtil::CurrentThreadId();
	FatAssertNoText(exclusiveOwningThreadId_ == threadId);

	if (recurseCounter_)
	{
		--recurseCounter_;
	}
	else
	{
		exclusiveOwningThreadId_ = THREADID_NULL;
		ReleaseSRWLockExclusive(&srwlock_);
	}
}

bool Mutex::TryLock()
{
	const ThreadId threadId = ThreadUtil::CurrentThreadId();
	if (exclusiveOwningThreadId_ == threadId)
	{
		++recurseCounter_;
		return true;
	}
	else
	{
		const bool ret = (TryAcquireSRWLockExclusive(&srwlock_) == TRUE);
		if (ret)
		{
			exclusiveOwningThreadId_ = threadId;
		}
		return ret;
	}
}

#else

Mutex::Mutex()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex_, &attr);
	pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&mutex_);
}

void Mutex::Lock()
{
	pthread_mutex_lock(&mutex_);
}

void Mutex::Unlock()
{
	pthread_mutex_unlock(&mutex_);
}

bool Mutex::TryLock()
{
	return pthread_mutex_trylock(&mutex_) == 0;
}

#endif

}
