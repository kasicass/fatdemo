#include "FatFramework/FatFramework.h"

#if defined(FAT_ENABLE_UNITTEST)

using namespace Fat;

class MutexTest
{
public:
	MutexTest() :
		sharedValue_(0)
	{
	}

	Bool Increment()
	{
		if (!mutex_.TryLock())
			return false;

		Bool ret = (++sharedValue_ == MaxValue);
		mutex_.Unlock();
		return ret;
	}

	Bool Test()
	{
		MutexFastLocker lock(mutex_);
		return (sharedValue_ == MaxValue);
	}

private:
	static const UInt32 MaxValue = 10000;

	MutexFast mutex_;
	UInt32 sharedValue_;
};

static void MutexTestThreadFunc(void* args)
{
	MutexTest& testObj = *((MutexTest*)args);

	while (!testObj.Increment())
	{
		Thread::Sleep(0);
	}

	FatTest(testObj.Test());
}

TEST_DECLARE(TestMutex)
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
		MutexTest testObj;
		ThreadPtr pThread = Thread::Create("MutexTest", MutexTestThreadFunc, &testObj);
		while (!testObj.Test())
		{
			Thread::Sleep(0);
		}

		FatTest(testObj.Test());
		pThread->Join();
	}
}

#endif
