#include "FatFramework/Kernel/Thread/MutexFast.h"

namespace Fat {

#if defined(FAT_OS_WINDOWS)

// SRW Lock (Slim Reader/Writer Lock)
// Faster + lighter than CriticalSection, and only enters into kernel mode if contented.

MutexFast::MutexFast() :
	srwlock_(SRWLOCK_INIT)
{
}

MutexFast::~MutexFast()
{
}

void MutexFast::Lock()
{
	AcquireSRWLockExclusive(&srwlock_);
}

void MutexFast::Unlock()
{
	ReleaseSRWLockExclusive(&srwlock_);
}

bool MutexFast::TryLock()
{
	return TryAcquireSRWLockExclusive(&srwlock_);
}

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

#endif

}
