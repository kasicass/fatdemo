#include "FatFramework/Kernel/Thread/ConditionVariable.h"
#include "FatFramework/Kernel/Thread/ThreadUtil.h"
#include "FatFramework/Kernel/Common/Assertion.h"

namespace Fat {

#if FAT_OS_WINDOWS

ConditionVariable::ConditionVariable() :
	cond_(CONDITION_VARIABLE_INIT)
{
}

ConditionVariable::~ConditionVariable()
{
}


void ConditionVariable::Wait(Mutex& lock)
{
	TimedWait(lock, INFINITE);
}

void ConditionVariable::Wait(MutexFast& lock)
{
	TimedWait(lock, INFINITE);
}

bool ConditionVariable::TimedWait(Mutex& lock, UInt32 millis)
{
	bool ret;
	FatAssertNoText(lock.recurseCounter_ == 0);
	lock.exclusiveOwningThreadId_ = THREADID_NULL;
	ret = (SleepConditionVariableSRW(&cond_, &lock.srwlock_, millis, ULONG(0)) == TRUE);
	lock.exclusiveOwningThreadId_ = ThreadUtil::CurrentThreadId();
	return ret;
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

void ConditionVariable::Wait(Mutex& lock)
{
	pthread_cond_wait(&cond_, &lock.mutex_);
}

void ConditionVariable::Wait(MutexFast& lock)
{
	pthread_cond_wait(&cond_, &lock.mutex_);
}

bool ConditionVariable::TimedWait(Mutex& lock, UInt32 millis)
{
	struct timespec sleepTime;
	sleepTime.tv_sec = (millis / 1000);
	sleepTime.tv_nsec = (millis % 1000) * 1000000;

	return (pthread_cond_timedwait(&cond_, &lock.mutex_, &sleepTime) == 0);
}

bool ConditionVariable::TimedWait(MutexFast& lock, UInt32 millis)
{
	struct timespec sleepTime;
	sleepTime.tv_sec = (millis / 1000);
	sleepTime.tv_nsec = (millis % 1000) * 1000000;

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
