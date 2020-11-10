#include "FatFramework/FatFramework.h"

#if FAT_ENABLE_UNITTEST

using namespace Fat;

struct TestData
{
	Event event;
	int value;
};

static void EventThreadEntry_0(void* args)
{
	struct TestData* pData = (struct TestData*)args;
	
	pData->value += 2;
	pData->event.Set();

	pData->event.Wait();
	pData->value += 4;
	pData->event.Set();
}

TEST_DECLARE(Thread_Event_Test)
{
	TestData data;
	data.value = 1;

	ThreadPtr thr0 = Thread::Create("EventTestThread_0", EventThreadEntry_0, &data);

	data.event.Wait();
	FatTest(data.value == 3);
	data.event.Set();

	data.event.Wait();
	FatTest(data.value == 7);

	thr0->Join();
	thr0 = NULL;
}

#endif
