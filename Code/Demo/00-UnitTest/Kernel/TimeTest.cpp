#include "FatFramework/FatFramework.h"

#if FAT_ENABLE_UNITTEST

using namespace Fat;

TEST_DECLARE(TestTime)
{
	const Time beginTime = Time::GetAppTime();
	Time zeroTime;

	FatTest(zeroTime.GetSeconds() == 0.f);
	FatTest(beginTime.GetSeconds() > 0.f);

	Thread::Sleep(1);
	FatTest(beginTime.GetSeconds() < Time::GetAppTime().GetSeconds());
}

TEST_DECLARE(TestFpsCounter)
{
	const F32 meanTime = 3.f;
	const F32 frameTime = 0.01f;
	const UInt32 requiredLoops = UInt32(meanTime / frameTime);
	const UInt32 estimatedFps = UInt32(1.f / frameTime);

	FpsCounter counter(meanTime);

	// first loop
	for (UInt32 i = 0; i < (requiredLoops+1); ++i)
	{
		counter.Update(frameTime);
	}

	FatTest(counter.GetFramePerSecond() == estimatedFps);

	// second loop
	for (UInt32 i = 0; i < requiredLoops; ++i)
	{
		counter.Update(frameTime);
	}

	FatTest(counter.GetFramePerSecond() == estimatedFps);
}

#endif
