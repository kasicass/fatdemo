#include "FatFramework.h"

#if defined(FAT_OS_WINDOWS)

namespace Fat {

ID3D9ResetWarner* theD3D9ResetWarner = NULL;

ID3D9ResetWarner::ID3D9ResetWarner()
{
	FatAssertNoText(theD3D9ResetWarner == NULL);
	theD3D9ResetWarner = this;
}

ID3D9ResetWarner::~ID3D9ResetWarner()
{
	FatAssert(videoMemoryResources_.empty(),
		L"%d video memory resources have not been destroyed", videoMemoryResources_.size());

	FatAssertNoText(theD3D9ResetWarner != NULL);
	theD3D9ResetWarner = NULL;
}

void ID3D9ResetWarner::SendPreReset()
{
	MutexFastLocker locker(listMutex_);
	for (auto it = videoMemoryResources_.begin(); it != videoMemoryResources_.end(); ++it)
	{
		(*it)->PreReset();
	}
}

void ID3D9ResetWarner::SendPostResetSucceed()
{
	MutexFastLocker locker(listMutex_);
	for (auto it = videoMemoryResources_.begin(); it != videoMemoryResources_.end(); ++it)
	{
		(*it)->PostResetSucceed();
	}
}

void ID3D9ResetWarner::SendPostResetFailed()
{
	MutexFastLocker locker(listMutex_);
	for (auto it = videoMemoryResources_.begin(); it != videoMemoryResources_.end(); ++it)
	{
		(*it)->PostResetFailed();
	}
}

void ID3D9ResetWarner::RegisterResetListener(ID3D9ResetListener* pListener)
{
	MutexFastLocker locker(listMutex_);
	videoMemoryResources_.push_front(pListener);
}

void ID3D9ResetWarner::UnregisterResetListener(ID3D9ResetListener* pListener)
{
	MutexFastLocker locker(listMutex_);
	videoMemoryResources_.remove(pListener);
}

//
// ID3D9ResetListener
//

ID3D9ResetListener::ID3D9ResetListener()
{
}

ID3D9ResetListener::~ID3D9ResetListener()
{
}

void ID3D9ResetListener::RegisterMe()
{
}

void ID3D9ResetListener::UnregisterMe()
{
}

}

#endif
