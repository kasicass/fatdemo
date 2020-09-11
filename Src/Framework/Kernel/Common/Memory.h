#pragma once

#if defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)

#define FatMalloc(size)     Fat::Memory::MallocDbg(size, FAT_CONCAT(L,__FILE__), __LINE__)
#define FatRealloc(p, size) Fat::Memory::ReallocDbg(p, size, FAT_CONCAT(L,__FILE__), __LINE__)
#define FatFree(p)          Fat::Memory::FreeDbg(p)

#else

#define FatMalloc(size)     Fat::Memory::MallocInternal(size)
#define FatRealloc(p, size) Fat::Memory::ReallocInternal(p, size)
#define FatFree(p)          Fat::Memory::FreeInternal(p)

#endif

#define FatNew(Type, ...) new Type(__VA_ARGS__)
#define FatNewArray(Type, Count) new Type[Count]
#define FatDelete(Pointer) delete Pointer
#define FatDeleteArray(Pointer) delete [] Pointer

namespace Fat {

namespace Memory {
	void Init();
	void Shutdown();

	void* MallocInternal(UInt32 size);
	void* ReallocInertnal(void* p, UInt32 size);
	void FreeInternal(void* p);

#if defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)
	void* MallocDbg(UInt32 size, const wchar_t* file, int line);
	void* ReallocDbg(void* p, UInt32 size, const wchar_t* file, int line);
	void FreeDbg(void* p);
#endif
}

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
