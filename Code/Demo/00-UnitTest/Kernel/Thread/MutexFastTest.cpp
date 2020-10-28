#include "FatFramework/FatFramework.h"

#if FAT_ENABLE_UNITTEST

using namespace Fat;

class MutexFastTest
{
public:
	MutexFastTest() :
		sharedValue_(0)
	{
	}

	bool Increment()
	{
		if (!mutex_.TryLock())
			return false;

		bool ret = (++sharedValue_ == MaxValue);
		mutex_.Unlock();
		return ret;
	}

	bool Test()
	{
		MutexFastLocker lock(mutex_);
		return (sharedValue_ == MaxValue);
	}

private:
	static const UInt32 MaxValue = 10000;

	MutexFast mutex_;
	UInt32 sharedValue_;
};

static void MutexFastTestThreadFunc(void* args)
{
	MutexFastTest& testObj = *((MutexFastTest*)args);

	while (!testObj.Increment())
	{
		ThreadUtil::Sleep(0);
	}

	FatTest(testObj.Test());
}

TEST_DECLARE(Thread_MutexFast_Test)
{
	{
		MutexFast mutex;
		MutexFastLocker lock(mutex);
	}

	{
		MutexFast mutex;
		mutex.TryLock();
		mutex.Unlock();
	}

	// test threaded exclusion
	{
		MutexFastTest testObj;
		ThreadPtr pThread = Thread::Create("MutexFastTest", MutexFastTestThreadFunc, &testObj);
		while (!testObj.Test())
		{
			ThreadUtil::Sleep(0);
		}

		FatTest(testObj.Test());
		pThread->Join();
	}
}

#endif
