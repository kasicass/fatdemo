#include "FatFramework.h"

namespace Fat {

#if defined(FAT_OS_WINDOWS)

ConditionVariable::ConditionVariable() :
	cond_(CONDITION_VARIABLE_INIT)
{
}

ConditionVariable::~ConditionVariable()
{
}

void ConditionVariable::Wait(MutexFast& lock)
{
	TimedWait(lock, INFINITE);
}

bool ConditionVariable::TimedWait(MutexFast& lock, UInt32 millis)
{
	return (SleepConditionVariableSRW(&cond_, &lock.srwlock_, millis, ULONG(0)) == TRUE);
}

void ConditionVariable::NotifyOne()
{
	WakeConditionVariable(&cond_);
}

void ConditionVariable::NotifyAll()
{
	WakeAllConditionVariable(&cond_);
}

#else

ConditionVariable::ConditionVariable()
{
	pthread_cond_init(&cond_, NULL);
}

ConditionVariable::~ConditionVariable()
{
	pthread_cond_destroy(&cond_);
}

void ConditionVariable::Wait(MutexFast& lock)
{
	pthread_cond_wait(&cond_, &lock.mutex_);
}

bool ConditionVariable::TimedWait(MutexFast& lock, UInt32 millis)
{
	struct timespec sleepTime;
	sleepTime.tv_sec  = (millis / 1000);
	sleepTime.tv_nsec = (millis % 1000)*1000000;

	return (pthread_cond_timedwait(&cond_, &lock.mutex_, &sleepTime) == 0);
}

void ConditionVariable::NotifyOne()
{
	pthread_cond_signal(&cond_);
}

void ConditionVariable::NotifyAll()
{
	pthread_cond_broadcast(&cond_);
}

#endif

}
