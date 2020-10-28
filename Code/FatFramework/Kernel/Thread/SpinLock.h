// SpinLocks are efficient if threads are only likely to be blocked for a short
// period of time, as they avoid overhead from operating system process re-scheduling
// or context switching. However, spinlocks become wasteful if held for longer durations,
// both preventing other threads from running and required re-schedulling. The longer
// a lock is held by a thread, the greater the risk that it will be interrupted by the
// OS scheduler while holding the lock. If this happens, other threads will be left
// "spinning" (repeatedly trying to acquire the lock), while the thread holding the
// lock is not making progress towards releasing it. The result is a semi-deadlock
// until the thread holding the lock can finish and release it. This is especially true
// on a single-processor system, where each waiting thread of the same priority is likely
// to waste its quantum (allocated time where a thread can run) spinning until th thread
// that holds the lock is finally finished.

#pragma once

#include "FatFramework/Kernel/Thread/Interlocked.h"

namespace Fat {

enum EWaitMode : bool
{
	Wait   = true,
	NoWait = false,
};

enum ESleepMode : bool
{
	Sleep   = true,
	NoSleep = false,
};

template <EWaitMode Wait, ESleepMode Sleep>
class SpinLock
{
public:
	FAT_FORCE_INLINE SpinLock() : lockSem_(0) {}

	FAT_FORCE_INLINE bool Lock()
	{
		do
		{
			// Atomically swap the lock variable with 1 if it's 
			if (Interlocked::CompareExchange32(&lockSem_, 1, 0))
			{
				// We successfully acquired the lock
				Interlocked::MemorySync();
				return true;
			}

			// To reduce inter-CPU bus traffic, when the lock is not acquired, loop reading
			// without trying to write anything, until the value changes. This optimization
			// should be effective on all CPU architectures that have a cache per CPU.
			while (Wait && lockSem_)
			{
				if (Sleep)
				{
					ThreadUtil::SwitchThread();
				}
			}
		} while (Wait);

		return false;
	}

	FAT_FORCE_INLINE void Unlock()
	{
		Interlocked::MemorySync();
		lockSem_ = 0;
	}

private:
	volatile SInt32 lockSem_;
};

}
