#pragma once

namespace Fat {

// recursive mutex
class Mutex : private NonCopyable
{
	friend class ConditionVariable;

public:
	Mutex();
	~Mutex();

	void Lock();
	void Unlock();
	bool TryLock();

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
