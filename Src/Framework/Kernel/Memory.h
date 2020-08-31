#pragma once

namespace Fat {

void* FatMalloc(UInt32 size, UInt32 align = 16);
void* FatRealloc(void* p, UInt32 size, UInt32 align = 16);
void FatFree(void* p);

#define FatNew(Type, ...) new Type(__VA_ARGS__)
#define FatNewArray(Type, Count) new Type[Count]
#define FatDelete(Pointer) delete Pointer
#define FatDeleteArray(Pointer) delete [] Pointer

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
