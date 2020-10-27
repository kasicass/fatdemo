#include "FatFramework/FatFramework.h"

#if FAT_ENABLE_UNITTEST

using namespace Fat;

class CondTest
{
public:
	CondTest() :
		sharedValue_(0),
		execute_(0),
		exit_(false)
	{
	}

	UInt32 sharedValue_;
	UInt32 execute_;
	Bool exit_;

	MutexFast mutex_;
	ConditionVariable cond_;
};

void TestCondThreadFunc(void* args)
{
	CondTest& data = *((CondTest*)args);

	MutexFastLocker lock(data.mutex_);

	while (1)
	{
		while (data.execute_ == 0 && data.exit_ != true)
		{
			data.cond_.Wait(data.mutex_);
		}

		if (data.exit_)
			return;

		if (data.execute_ != 0)
		{
			data.execute_--;
			data.sharedValue_++;

			data.cond_.NotifyAll();
		}
	}
}

TEST_DECLARE(TestConditionVariable)
{
	// Test basic single threaded condition
	{
		Bool bCond = false;

		MutexFast mutex;
		ConditionVariable cond;

		MutexFastLocker lock(mutex);
		while (!bCond)
		{
			Bool timedWaitRet = cond.TimedWait(mutex, 1);
			FatTest(timedWaitRet == false); // should fail has no notify occurs
			bCond = true;
		}
	}

	// Test threaded notify waits
	{
		CondTest data;

		ThreadPtr threads[] = {
			Thread::Create("TestCond", TestCondThreadFunc, &data),
			Thread::Create("TestCond", TestCondThreadFunc, &data),
			Thread::Create("TestCond", TestCondThreadFunc, &data),
			Thread::Create("TestCond", TestCondThreadFunc, &data),
			Thread::Create("TestCond", TestCondThreadFunc, &data),
			Thread::Create("TestCond", TestCondThreadFunc, &data),
			Thread::Create("TestCond", TestCondThreadFunc, &data),
			Thread::Create("TestCond", TestCondThreadFunc, &data),
			Thread::Create("TestCond", TestCondThreadFunc, &data),
			Thread::Create("TestCond", TestCondThreadFunc, &data),
		};

		// Ask for execution using notify one
		{
			MutexFastLocker lock(data.mutex_);

			// Initial value is 0
			FatTest(data.sharedValue_ == 0);

			data.execute_++;
			data.cond_.NotifyOne();

			// sharedValue_ still equals 0 as mutex isn't unlocked
			FatTest(data.sharedValue_ == 0);
		}

		// Wait execution ends
		{
			MutexFastLocker lock(data.mutex_);
			while (data.execute_ != 0)
			{
				data.cond_.Wait(data.mutex_);
			}

			// Now sharedValue_ is incremented
			FatTest(data.sharedValue_ == 1);
		}

		// Ask again for more executions using notify all
		{
			MutexFastLocker lock(data.mutex_);
			data.execute_ = FAT_ARRAY_SIZE(threads);
			data.cond_.NotifyAll();

			// sharedValue_ still equals 1 as mutex isn't unlocked
			FatTest(data.sharedValue_ == 1);
		}

		// Wait execution ends
		{
			MutexFastLocker lock(data.mutex_);
			while (data.execute_ != 0)
			{
				data.cond_.Wait(data.mutex_);
			}

			FatTest(data.sharedValue_ == (1 + FAT_ARRAY_SIZE(threads)));
		}

		// Ask for exit using notify all
		{
			MutexFastLocker lock(data.mutex_);
			data.exit_ = true;
			data.cond_.NotifyAll();
		}

		// Join threads
		for (int i = 0; i < FAT_ARRAY_SIZE(threads); ++i)
		{
			threads[i]->Join();
		}
	}
}

#endif
