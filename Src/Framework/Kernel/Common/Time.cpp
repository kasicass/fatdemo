#include "FatFramework.h"

namespace Fat {

#if defined(FAT_OS_WINDOWS)

class PerformanceCounter : public IPerformanceCounter
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	virtual F32 ComputeAppTime() const override;

private:
	F64 invFrequency_;
	Int64 initCounter_;
};

void PerformanceCounter::Init()
{
	FatLog(L"<PerfCounter>: Init");

	Int64 freq;
	FatValidateNoText(QueryPerformanceFrequency((LARGE_INTEGER*)&freq) != 0);
	invFrequency_ = 1.0 / F64(freq);

	FatValidateNoText(QueryPerformanceCounter((LARGE_INTEGER*)&initCounter_) != 0);
}

void PerformanceCounter::Shutdown()
{
	FatLog(L"<PerfCounter>: Shutdown");
}

F32 PerformanceCounter::ComputeAppTime() const
{
	Int64 currentCounter;
	FatValidateNoText(QueryPerformanceCounter((LARGE_INTEGER*)&currentCounter) != 0);

	const F64 deltaCounter = F64(currentCounter - initCounter_);
	const F64 deltaTime = deltaCounter * invFrequency_;

	return F32(deltaTime);
}

#else

class PerformanceCounter : public IPerformanceCounter
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	virtual F32 ComputeAppTime() const override;

private:
	static Int64 GetCurrentTicks();

	F64 invFrequency_;
	Int64 initCounter_;
};

Int64 PerformanceCounter::GetCurrentTicks()
{
	timespec tv;
	clock_gettime(CLOCK_MONOTONIC, &tv);
	return (Int64)tv.tv_sec*1000000 + tv.tv_nsec/1000;
}

void PerformanceCounter::Init()
{
	FatLog(L"<PerfCounter>: Init");

	invFrequency_ = 1.0 / F64(1000000);
	initCounter_  = GetCurrentTicks();
}

void PerformanceCounter::Shutdown()
{
	FatLog(L"<PerfCounter>: Shutdown");
}

F32 PerformanceCounter::ComputeAppTime() const
{
	Int64 currentCounter = GetCurrentTicks();

	const F64 deltaCounter = F64(currentCounter - initCounter_);
	const F64 deltaTime = deltaCounter * invFrequency_;

	return F32(deltaTime);
}

#endif

//
// IPerformanceCounter Singleton
//

static PerformanceCounter myPerfCounter;
IPerformanceCounter* thePerfCounter = &myPerfCounter;

//
// Time
//

Time Time::GetAppTime()
{
	F32 timeInSeconds = thePerfCounter->ComputeAppTime();
	return Time(timeInSeconds);
}

Time::Time() :
	seconds_(0.f)
{
}

Time::Time(F32 seconds) :
	seconds_(seconds)
{
}

Time::Time(const Time& rhs) :
	seconds_(rhs.seconds_)
{
}
Time& Time::operator=(const Time& rhs)
{
	seconds_ = rhs.seconds_;
	return *this;
}

F32 Time::GetSeconds() const
{
	return seconds_;
}

F32 Time::GetMilliSeconds() const
{
	return seconds_ * 1000.f;
}

//
// FpsCounter
//

FpsCounter::FpsCounter(F32 meanTime) :
	accumulatedTime_(0),
	meanTime_(meanTime),
	frameCount_(0),
	framePerSecond_(0)
{
}

void FpsCounter::Update(F32 frameTime)
{
	accumulatedTime_ += frameTime;
	++frameCount_;

	if (accumulatedTime_ > meanTime_)
	{
		// compute fps
		framePerSecond_ = UInt32(F32(frameCount_) / accumulatedTime_);

		// reset counters
		frameCount_ = 1;
		accumulatedTime_ = frameTime;
	}
}

UInt32 FpsCounter::GetFramePerSecond() const
{
	return framePerSecond_;
}

//
// FpsAutoCounter
//

FpsAutoCounter::FpsAutoCounter(F32 meanTime) :
	FpsCounter(meanTime),
	lastTime_(0)
{
}

void FpsAutoCounter::Update()
{
	F32 currentTime = Time::GetAppTime().GetSeconds();
	F32 deltaTime = currentTime - lastTime_;
	FpsCounter::Update(deltaTime);
	lastTime_ = currentTime;
}

//
// Time - UnitTest
//

#if defined(FAT_ENABLE_UNITTEST)

TEST(TestTime)
{
	const Time beginTime = Time::GetAppTime();
	Time zeroTime;

	FatTest(zeroTime.GetSeconds() == 0.f);
	FatTest(beginTime.GetSeconds() > 0.f);
	FatTest(beginTime.GetSeconds() < Time::GetAppTime().GetSeconds());
}

TEST(TimeFpsCounter)
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

}

