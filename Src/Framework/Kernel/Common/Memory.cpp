#include "FatFramework.h"

namespace Fat {

#if defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)

// Memory leak detection
static void DetectMemoryLeak(ERegisteredOperation::EValue op)
{
	static _CrtMemState s_bootMemState;

	if (op == ERegisteredOperation::eInitialize)
	{
		// Perform automatic leak checking at program exit
		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		// Store the memory state at application boot time
		_CrtMemCheckpoint(&s_bootMemState);
	}
	else if (op == ERegisteredOperation::eDestory)
	{
		// Store the current memory state
		_CrtMemState currMemState;
		_CrtMemCheckpoint(&currMemState);

		// Compute memory state difference since boot
		_CrtMemState diffMemState;
		if (_CrtMemDifference(&diffMemState, &s_bootMemState, &currMemState))
		{
			// Dump all unfreed allocation since boot time
			_CrtMemDumpAllObjectsSince(&s_bootMemState);

			// Inform user of the leaks
			FatAssert(false, L"Memory leaks detected, see debugger output");
		}
	}
}

FAT_REGISTER_FUNCTION(&DetectMemoryLeak, ERegisterPrimaryPriority::eReserved, ERegisterSecondaryPriority::eHighest);

#endif

void MemoryCopy(void* pDest, const void* pSource, UInt32 count)
{
	memcpy(pDest, pSource, count);
}

void MemorySet(void* pDest, Int32 ch, UInt32 count)
{
	memset(pDest, ch, count);
}

}
