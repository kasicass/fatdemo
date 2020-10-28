#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/PlatformHeaders.h"
#include "FatFramework/Kernel/Common/Types.h"

namespace Fat {

class ThreadUtil
{
public:
	FAT_FORCE_INLINE static void Sleep(UInt32 milliSeconds);
	FAT_FORCE_INLINE static void SwitchThread();
	FAT_FORCE_INLINE static ThreadId CurrentThreadId();
};

#if FAT_OS_WINDOWS

FAT_FORCE_INLINE void ThreadUtil::Sleep(UInt32 milliSeconds)
{
	::Sleep(milliSeconds);
}

FAT_FORCE_INLINE void ThreadUtil::SwitchThread()
{
	::Sleep(0);
}

FAT_FORCE_INLINE ThreadId ThreadUtil::CurrentThreadId()
{
	return (ThreadId)::GetCurrentThreadId();
}

#else

FAT_FORCE_INLINE void ThreadUtil::Sleep(UInt32 milliSeconds)
{
	struct timespec sleepTime;
	sleepTime.tv_sec = (milliSeconds / 1000);
	sleepTime.tv_nsec = (milliSeconds % 1000) * 1000000;

	::nanosleep(&sleepTime, NULL);
}

FAT_FORCE_INLINE void ThreadUtil::SwitchThread()
{
	Sleep(0);
}

FAT_FORCE_INLINE ThreadId ThreadUtil::CurrentThreadId()
{
	return (ThreadId)pthread_self();
}

#endif

}
