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

#endif

}
