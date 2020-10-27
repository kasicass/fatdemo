#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/Common/Types.h"

#if FAT_ENABLE_MEMORY_LEAK_DETECTION

#include "FatFramework/Kernel/Common/Macros.h"

#define FatMalloc(size)          Fat::Memory::MallocDbg(size, FAT_CONCAT(L,__FILE__), __LINE__)
#define FatRealloc(p, size)      Fat::Memory::ReallocDbg(p, size, FAT_CONCAT(L,__FILE__), __LINE__)
#define FatFree(p)               Fat::Memory::FreeDbg(p)

#define FatNew(Type, ...)        new(FAT_CONCAT(L,__FILE__), __LINE__) Type(__VA_ARGS__)
#define FatNewArray(Type, Count) new(FAT_CONCAT(L,__FILE__), __LINE__) Type[Count]
#define FatDelete(Pointer)       delete Pointer
#define FatDeleteArray(Pointer)  delete [] Pointer

void operator delete(void* p, const wchar_t* file, int line) noexcept;
void operator delete[](void* p, const wchar_t* file, int line) noexcept;

void* operator new(size_t n, const wchar_t* file, int line) noexcept(false);
void* operator new[](size_t n, const wchar_t* file, int line) noexcept(false);

#else

#define FatMalloc(size)          Fat::Memory::MallocInternal(size)
#define FatRealloc(p, size)      Fat::Memory::ReallocInternal(p, size)
#define FatFree(p)               Fat::Memory::FreeInternal(p)

#define FatNew(Type, ...)        new Type(__VA_ARGS__)
#define FatNewArray(Type, Count) new Type[Count]
#define FatDelete(Pointer)       delete Pointer
#define FatDeleteArray(Pointer)  delete [] Pointer

#endif

namespace Fat {

namespace Memory {
	void Init();
	void Shutdown();

	void* MallocInternal(size_t size);
	void* ReallocInternal(void* p, size_t size);
	void FreeInternal(void* p);

#if FAT_ENABLE_MEMORY_LEAK_DETECTION
	void* MallocDbg(size_t size, const wchar_t* file, int line);
	void* ReallocDbg(void* p, size_t size, const wchar_t* file, int line);
	void FreeDbg(void* p);
#endif
}

void MemoryCopy(void* pDest, const void* pSource, UInt32 count);
void MemorySet(void* pDest, SInt32 ch, UInt32 count);

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
