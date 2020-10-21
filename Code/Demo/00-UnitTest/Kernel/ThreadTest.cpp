#include "FatFramework/FatFramework.h"

#if defined(FAT_ENABLE_UNITTEST)

using namespace Fat;

TEST_DECLARE(TestThreadOperators)
{
	ThreadPtr pThread1 = Thread::GetCurrent();
	ThreadPtr pThread2 = Thread::GetCurrent();

	FatTest(pThread1 == pThread2);
	FatTest((pThread1 != pThread2) == false);
}

static void ThreadEmptyEntry(void* args)
{
	FatTest(args == (void*)0x12345678);
}

TEST_DECLARE(TestThreadCreation)
{
	ThreadPtr pThread1 = Thread::GetCurrent();

	FatTestAssert(Thread::Attach("Thread attached twice"));

	ThreadPtr pThread2 = Thread::Create("TestThread", ThreadEmptyEntry, (void*)0x12345678);

	FatTestAssert(FatAssert(false, L"Assert in MainThread"));

	FatTest(pThread1 != pThread2);

	pThread2->Join();
}


//
// Multiple simultaneous calls to Thread::Join()
//

static volatile int g_joinValue = 0;

static void ThreadEntry_0(void* args)
{
	g_joinValue = 1;
	Thread::Sleep(50);
	g_joinValue = 2;
}

struct ThreadData_1
{
	ThreadPtr pMainThread;
	ThreadPtr pThread_0;
};

static void ThreadEntry_1(void* args)
{
	ThreadData_1& data = *((ThreadData_1*)args);

	Thread::Sleep(5);
	FatTest(g_joinValue == 1);

	data.pThread_0->Join();
	FatTest(Thread::GetCurrent() != data.pMainThread);
	FatTest(g_joinValue == 2);

	g_joinValue = 3;
	Thread::Sleep(5);
}

TEST_DECLARE(TestThreadMutiJoin)
{
	// mainthread - create thread_0, thread_1
	// thread_0 - do sthing
	// mainthread/thread_1 - simultaneous call thread_0.join() 
	ThreadPtr thr0 = Thread::Create("TestThread_0", ThreadEntry_0, NULL);

	ThreadData_1 data1;
	data1.pMainThread = Thread::GetCurrent();
	data1.pThread_0   = thr0;
	ThreadPtr thr1 = Thread::Create("TestThread_1", ThreadEntry_1, &data1);

	thr0->Join();
	thr0 = NULL;

	thr1->Join();
	thr1 = NULL;
	FatTest(g_joinValue == 3);
}

#endif
