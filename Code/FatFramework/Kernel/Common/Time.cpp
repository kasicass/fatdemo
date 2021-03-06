#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/PlatformHeaders.h"
#include "FatFramework/Kernel/Common/Time.h"
#include "FatFramework/Kernel/Common/Log.h"
#include "FatFramework/Kernel/Common/Assertion.h"

namespace Fat {

#if FAT_OS_WINDOWS

class PerformanceCounter : public IPerformanceCounter
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	virtual F32 ComputeAppTime() const override;

private:
	F64 invFrequency_;
	SInt64 initCounter_;
};

void PerformanceCounter::Init()
{
	FatLog(L"<PerfCounter>: Init");

	SInt64 freq;
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
	SInt64 currentCounter;
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
	static SInt64 GetCurrentTicks();

	F64 invFrequency_;
	SInt64 initCounter_;
};

SInt64 PerformanceCounter::GetCurrentTicks()
{
	timespec tv;
	clock_gettime(CLOCK_MONOTONIC, &tv);
	return (SInt64)tv.tv_sec*1000000 + tv.tv_nsec/1000;
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
	SInt64 currentCounter = GetCurrentTicks();

	const F64 deltaCounter = F64(currentCounter - initCounter_);
	const F64 deltaTime = deltaCounter * invFrequency_;

	return F32(deltaTime);
}

#endif

//
// IPerformanceCounter Singleton
//

static PerformanceCounter myPerfCounter;
IPerformanceCounter* GPerfCounter = &myPerfCounter;

//
// Time
//

Time Time::GetAppTime()
{
	F32 timeInSeconds = GPerfCounter->ComputeAppTime();
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

UInt32 FpsAutoCounter::GetFramePerSecond() const
{
	return FpsCounter::GetFramePerSecond();
}

}
