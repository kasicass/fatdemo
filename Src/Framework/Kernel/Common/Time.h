#pragma once

#include "Kernel/Common/Types.h"
#include "Kernel/Common/NonCopyable.h"

namespace Fat {

struct IPerformanceCounter
{
	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual F32 ComputeAppTime() const = 0;
};
extern IPerformanceCounter* thePerfCounter;



class Time
{
public:
	static Time GetAppTime();

public:
	Time();
	explicit Time(F32 seconds);

	Time(const Time& rhs);
	Time& operator=(const Time& rhs);

	F32 GetSeconds() const;
	F32 GetMilliSeconds() const;

private:
	F32 seconds_;
};

// Frame per second. Update a fps counter every time a certain time has elapsed
class FpsCounter : private NonCopyable
{
public:
	FpsCounter(F32 meanTime = 0.5f);

	// Update to next frame, compute the fps if mean time has been elapsed
	void Update(F32 frameTime);

	UInt32 GetFramePerSecond() const;

private:
	F32 accumulatedTime_;   // accumulated time since last fps reset
	F32 meanTime_;          // time elapsed between fps counter updates
	UInt32 frameCount_;     // frame count between every fps counter updates
	UInt32 framePerSecond_; // last computed fps
};

// Frame per second automatic counter. Update a fps counter every time a certain time has elapsed
class FpsAutoCounter : private FpsCounter
{
public:
	FpsAutoCounter(F32 meanTime= 1.f);

	// Update to next frame, compute the fps if mean time has been elapsed
	void Update();

	UInt32 GetFramePerSecond() const;

private:
	F32 lastTime_; // time at which the fps was updated last
};

}
