#pragma once

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
#if defined(FAT_ENABLE_ASSERT)
	bool IsLocked() const;
#endif

private:
	MutexFast(const MutexFast& rhs);
	MutexFast& operator=(const MutexFast& rhs);

private:
#if defined(FAT_OS_WINDOWS)
	SRWLOCK srwlock_;
#else
	pthread_mutex_t mutex_;
#endif

	FatIfBuildAssertion(Bool isLocked_);
};

}

typedef Fat::TAutoLock<Fat::MutexFast> MutexFastLocker;
