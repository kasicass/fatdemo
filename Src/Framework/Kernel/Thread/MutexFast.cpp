#include "FatFramework.h"

namespace Fat {

#if defined(FAT_OS_WINDOWS)

// SRW Lock (Slim Reader/Writer Lock)
// Faster + lighter than CriticalSection, and only enters into kernel mode if contented.

MutexFast::MutexFast() :
	srwlock_(SRWLOCK_INIT)
	FatIfBuildAssertion(, isLocked_(false))
{
}

MutexFast::~MutexFast()
{
	FatAssert(isLocked_ == false, L"Mutex should already be unlocked at destruction time");
}

void MutexFast::Lock()
{
	AcquireSRWLockExclusive(&srwlock_);

	FatAssert(isLocked_ == false, L"Mutex should be unlocked right now");
	FatIfBuildAssertion(isLocked_ = true);
}

void MutexFast::Unlock()
{
	FatAssert(isLocked_ == true, L"Unlocking a mutex that isn't locked");
	FatIfBuildAssertion(isLocked_ = false);

	ReleaseSRWLockExclusive(&srwlock_);
}

bool MutexFast::TryLock()
{
	if (TryAcquireSRWLockExclusive(&srwlock_))
	{
		FatAssert(isLocked_ == false, L"Mutex should be unlocked right now");
		FatIfBuildAssertion(isLocked_ = true);

		return true;
	}

	return false;
}

#if defined(FAT_ENABLE_ASSERT)
bool MutexFast::IsLocked() const
{
	return isLocked_;
}
#endif

#else

MutexFast::MutexFast()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
	pthread_mutex_init(&mutex_, &attr);
	pthread_mutexattr_destroy(&attr);
}

MutexFast::~MutexFast()
{
	pthread_mutex_destroy(&mutex_);
}

void MutexFast::Lock()
{
	pthread_mutex_lock(&mutex_);
}

void MutexFast::Unlock()
{
	pthread_mutex_unlock(&mutex_);
}

bool MutexFast::TryLock()
{
	return pthread_mutex_trylock(&mutex_) == 0;
}

#if defined(FAT_ENABLE_ASSERT)
bool MutexFast::IsLocked() const
{
	return isLocked_;
}
#endif

#endif

}
