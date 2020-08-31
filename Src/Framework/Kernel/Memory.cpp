#include "FatFramework.h"

namespace Fat {

#if defined(FAT_DEBUG_BUILD)

// Memory leak detection
// TODO

#endif

void* FatMalloc(UInt32 size, UInt32 align)
{
	void* p = _aligned_malloc(size, align);
	FatAssert(p != NULL, L"aligned_malloc failed");
	return p;
}

void* FatRealloc(void* p, UInt32 size, UInt32 align)
{
	void* pNew = _aligned_realloc(p, size, align);
	FatAssert(pNew != NULL, L"aligned_realloc failed");
	return pNew;
}

void FatFree(void* p)
{
	_aligned_free(p);
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
