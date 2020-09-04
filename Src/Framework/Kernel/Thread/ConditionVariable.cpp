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
	lock.exclusiveOwningThreadId_ = ::GetCurrentThreadId();
	return ret;
}

bool ConditionVariable::TimedWait(MutexFast& lock, UInt32 millis)
{
	return (SleepConditionVariableSRW(&cond_, &lock.srwlock_, millis, ULONG(0)) == TRUE);
}

void ConditionVariable::NotifySingle()
{
	WakeConditionVariable(&cond_);
}

void ConditionVariable::NotifyAll()
{
	WakeAllConditionVariable(&cond_);
}

#else

#endif

}
