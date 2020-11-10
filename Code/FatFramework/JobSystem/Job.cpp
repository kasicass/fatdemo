#include "FatFramework/JobSystem/Job.h"
#include "FatFramework/JobSystem/JobManager.h"
#include "FatFramework/Kernel/Common/Assertion.h"
#include "FatFramework/Kernel/Thread/Interlocked.h"

namespace Fat {

Job::Job(bool autoDestroy, bool waitEvent, volatile SInt32* execCounter)
{
	scheduled_    = false;
	executed_     = false;
	autoDestroy_  = autoDestroy;
	isDependency_ = false;
	isHashed_     = false;
	execCounter_  = execCounter;
	syncCounter_  = 0;
	hashNext_     = NULL;

	FatAssertNoText(!waitEvent || !autoDestroy);

	static UInt32 UniqueJobId = 0;
	jobId_ = Interlocked::Increment32((SInt32*)&UniqueJobId);

	waitEvent_ = waitEvent ? new Event() : NULL;
}

Job::~Job()
{
	FAT_SAFE_DELETE(waitEvent_);
}

void Job::OnExecuted()
{
	if (execCounter_)
	{
		Interlocked::Decrement32(execCounter_);
	}

	if (waitEvent_)
	{
		waitEvent_->Set();
	}
}

void Job::Trigger(JobManager* jobManager)
{
	Execute(jobManager);

	executed_ = true;

	if (!isHashed_ && IsExecuted())
	{
		OnExecuted();

		if (autoDestroy_)
		{
			delete this;
		}
	}
	else
	{
		// TODO
		// FatValidateNoText(jobManager->PendingTasksQueue.Enqueue(this));
	}
}

}
