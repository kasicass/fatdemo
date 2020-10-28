#include "FatFramework/FatFramework.h"

#if FAT_ENABLE_UNITTEST

using namespace Fat;

struct ThreadData
{
	SpinLock<EWaitMode::Wait, ESleepMode::Sleep> lock;
	int value;
};

static void SpinLockThreadFunc01(void* args)
{
	ThreadData& data = *(ThreadData*)(args);
	data.lock.Lock();
	data.value++;
	data.lock.Unlock();
}

static void SpinLockThreadFunc02(void* args)
{
	ThreadData& data = *(ThreadData*)(args);
	data.lock.Lock();
	data.value += 2;
	data.lock.Unlock();
}

TEST_DECLARE(Thread_SpinLock_Test)
{
	bool ok;

	// basic usage
	{
		ThreadData data;
		data.value = 10;

		ok = data.lock.Lock();
		FatTest(ok);
		data.lock.Unlock();

		ThreadPtr thr0 = Thread::Create("SpinLockTestThread_01", SpinLockThreadFunc01, (void*)&data);
		ThreadPtr thr1 = Thread::Create("SpinLockTestThread_02", SpinLockThreadFunc02, (void*)&data);

		thr0->Join();
		thr1->Join();

		FatTest(13 == data.value);
	}

	// no wait, no sleep
	{
		SpinLock<EWaitMode::NoWait, ESleepMode::NoSleep> lock;

		ok = lock.Lock();
		FatTest(ok);

		ok = lock.Lock();
		FatTest(!ok);

		lock.Unlock();
		ok = lock.Lock();
		FatTest(ok);
	}

	// wait, no sleep
	{
		SpinLock<EWaitMode::Wait, ESleepMode::NoSleep> lock;

		ok = lock.Lock();
		FatTest(ok);

		lock.Unlock();
		ok = lock.Lock();
		FatTest(ok);
	}
}

#endif
