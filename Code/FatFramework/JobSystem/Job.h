#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/PlatformHeaders.h"
#include "FatFramework/Kernel/Common/Types.h"
#include "FatFramework/Kernel/Thread/Event.h"

namespace Fat {

class JobManager;

class Job
{
	friend class JobManager;

protected:
	void Trigger(JobManager* jobManager);

	virtual SInt32 GetDependencies(Job**& dependencies)
	{
		dependencies = 0;
		return 0;
	}

	// Called by the scheduler when the task is fully executed
	virtual void OnExecuted();

	UInt32 jobId_;                      // Internal job-id, used for job hashing
	Job* hashNext_;                     // Internal hashmap next element pointer
	volatile SInt32* execCounter_;      // Pointer to a variable that gets decremented when execution is done
	volatile SInt32* syncCounter_;      // Used to wait for subtasks to complete
	Event* waitEvent_;                  // Event used to wait for a job to complete
	bool scheduled_ : 1;                // Is this job scheduled?
	bool executed_ : 1;                 // Is this job executed?
	bool autoDestroy_ : 1;              // Is this job automatically destroyed after execution?
	bool isDependency_ : 1;             // Is this job a dependency for another task?
	bool isHashed_ : 1;                 // Is this job in the dependencies hashmap?

public:
	Job(bool autoDestroy = true, bool waitEvent = false, volatile SInt32* exeCounter = NULL);
	virtual ~Job();

	virtual void Execute(JobManager* jobManager) = 0;

	FAT_FORCE_INLINE bool IsExecuted() const
	{
		return executed_ && !syncCounter_;
	}

	FAT_FORCE_INLINE void Sync()
	{
		waitEvent_->Wait();
	}
};

}
