#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/Common/Types.h"

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
};

}
