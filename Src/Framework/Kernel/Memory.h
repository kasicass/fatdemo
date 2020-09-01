#pragma once

// https://www.flipcode.com/archives/Detecting_Memory_Leaks.shtml
#if defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)

#define _FatMalloc(size, align) _aligned_malloc_dbg(size, align, __FILE__, __LINE__)
#define FatMalloc(size) _FatMalloc(size, 16)

#define _FatRealloc(p, size, align) _aligned_realloc_dbg(p, size, align, __FILE__, __LINE__)
#define FatRealloc(p, size) _FatRealloc(p, size)

#define FatFree(p) _aligned_free_dbg(p)

#define FatNew(Type, ...) new(_NORMAL_BLOCK, __FILE__, __LINE__) Type(__VA_ARGS__)
#define FatNewArray(Type, Count) new(_NORMAL_BLOCK, __FILE__, __LINE__) Type[Count]
#define FatDelete(Pointer) delete Pointer
#define FatDeleteArray(Pointer) delete [] Pointer

#else

inline void* FatMalloc(UInt32 size, UInt32 align = 16)
{
	void* p = _aligned_malloc(size, align);
	FatAssert(p != NULL, L"Malloc failed");
	return p;
}

inline void* FatRealloc(void* p, UInt32 size, UInt32 align = 16)
{
	void* pNew = _aligned_realloc(p, size, align);
	FatAssert(pNew != NULL, L"Realloc failed");
	return p;
}

inline void FatFree(void* p)
{
	_aligned_free(p);
}

#define FatNew(Type, ...) new Type(__VA_ARGS__)
#define FatNewArray(Type, Count) new Type[Count]
#define FatDelete(Pointer) delete Pointer
#define FatDeleteArray(Pointer) delete [] Pointer

#endif

namespace Fat {

void MemoryCopy(void* pDest, const void* pSource, UInt32 count);
void MemorySet(void* pDest, Int32 ch, UInt32 count);

inline void MemoryZero(void* pDest, UInt32 count)
{
	MemorySet(pDest, 0, count);
}

template <typename T>
inline void MemoryZero(T& obj)
{
	MemoryZero(&obj, (UInt32)sizeof(T));
}

}
