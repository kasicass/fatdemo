#pragma once

namespace Fat {

class ConditionVariable : private NonCopyable
{
public:
	ConditionVariable();
	~ConditionVariable();

	void Wait(MutexFast& lock);
	bool TimedWait(MutexFast& lock, UInt32 millis);
	void NotifyOne();
	void NotifyAll();

private:
#if defined(FAT_OS_WINDOWS)
	CONDITION_VARIABLE cond_;
#else
	pthread_cond_t cond_;
#endif
};

}