#include "FatFramework.h"

namespace Fat {

#if !defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)
#if defined(FAT_OS_WINDOWS)

void* FatMalloc(UInt32 size, UInt32 align)
{
	void* p = _aligned_malloc(size, align);
	FatAssert(p != NULL, L"Malloc failed");
	return p;
}

void FatFree(void* p)
{
	_aligned_free(p);
}

#else

void* FatMalloc(UInt32 size, UInt32 align)
{
	void* p;
	int ret = posix_memalign(&p, align, size);
	FatAssert(ret == 0, L"Malloc failed");
	return p;
}

void FatFree(void* p)
{
	free(p);
}

#endif
#endif

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
