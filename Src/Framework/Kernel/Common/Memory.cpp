#include "FatFramework.h"

namespace Fat {

class MemoryManager : public IMemoryManager
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
#if defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)
	_CrtMemState bootMemState_;
#endif
};

void MemoryManager::Init()
{
#if defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)
	// Perform automatic leak checking at program exit
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Store the memory state at application boot time
	_CrtMemCheckpoint(&bootMemState_);
#endif
}

void MemoryManager::Shutdown()
{
#if defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)
	// Store the current memory state
	_CrtMemState currMemState;
	_CrtMemCheckpoint(&currMemState);

	// Compute memory state difference since boot
	_CrtMemState diffMemState;
	if (_CrtMemDifference(&diffMemState, &bootMemState_, &currMemState))
	{
		// Dump all unfreed allocation since boot time
		_CrtMemDumpAllObjectsSince(&bootMemState_);

		// Inform user of the leaks
		FatAssert(false, L"Memory leaks detected, see debugger output");
	}
#endif
}

static MemoryManager myMemMgr;
IMemoryManager* theMemMgr = &myMemMgr;

void MemoryCopy(void* pDest, const void* pSource, UInt32 count)
{
	memcpy(pDest, pSource, count);
}

void MemorySet(void* pDest, Int32 ch, UInt32 count)
{
	memset(pDest, ch, count);
}

}
