#include "FatFramework.h"

namespace Fat {

void InitMemoryCheck()
{
#if defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)
	FatLog(L"<MemCheck>: Init");

	// Perform automatic leak checking at program exit
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

void MemoryCopy(void* pDest, const void* pSource, UInt32 count)
{
	memcpy(pDest, pSource, count);
}

void MemorySet(void* pDest, Int32 ch, UInt32 count)
{
	memset(pDest, ch, count);
}

}
