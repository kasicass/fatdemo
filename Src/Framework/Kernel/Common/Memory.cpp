#include "FatFramework.h"
#include "Kernel/Common/Queue.h"

//
// new/delete override
//

#if defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)

// https://stackoverflow.com/questions/58694487/no-matching-operator-delete-found-memory-will-not-be-freed-if-initialization-th

// debug delete - called when class constructor throw exception
void operator delete(void* p, const wchar_t* file, int line) noexcept         { return Fat::Memory::FreeDbg(p); }
void operator delete[](void* p, const wchar_t* file, int line) noexcept       { return Fat::Memory::FreeDbg(p); }

// debug new
void* operator new(size_t n, const wchar_t* file, int line) noexcept(false)   { return Fat::Memory::MallocDbg(n, file, line); }
void* operator new[](size_t n, const wchar_t* file, int line) noexcept(false) { return Fat::Memory::MallocDbg(n, file, line); }

// normal delete
void operator delete(void* p) noexcept                                        { Fat::Memory::FreeDbg(p); }
void operator delete[](void* p) noexcept                                      { Fat::Memory::FreeDbg(p); }

// normal new
void* operator new(size_t n) noexcept(false)                                  { return Fat::Memory::MallocDbg(n, L"null", -1); }
void* operator new[](size_t n) noexcept(false)                                { return Fat::Memory::MallocDbg(n, L"null", -1); }

#else

// normal delete
void operator delete(void* p) noexcept                                        { Fat::Memory::FreeInternal(p); }
void operator delete[](void* p) noexcept                                      { Fat::Memory::FreeInternal(p); }

// normal new
void* operator new(size_t n) noexcept(false)                                  { return Fat::Memory::MallocInternal(n); }
void* operator new[](size_t n) noexcept(false)                                { return Fat::Memory::MallocInternal(n); }

#endif


namespace Fat {

namespace Memory {

//
// Malloc/Realloc/Free Dbg - Memory Leak Detection
//

#if defined(FAT_ENABLE_MEMORY_LEAK_DETECTION)

#define MAX_MALLOC_RECORDS (1000000)
#define INVALID_POINTER    ((void*)0x0BADBEEF)

struct MallocRecord {
	LIST_ENTRY(MallocRecord) list;
	void* ptr;
	const wchar_t* file;
	UInt32 line;
	UInt32 bytes;
};

LIST_HEAD(MallocRecordList, MallocRecord);

static volatile bool s_mallocRecordInit = false;
static UInt32 s_peakAllocCount = 0;
static UInt32 s_peakAllocBytes = 0;
static UInt32 s_currAllocCount = 0;
static UInt32 s_currAllocBytes = 0;
static MallocRecord s_mallocRecords[MAX_MALLOC_RECORDS];
static MallocRecordList s_freeList = { NULL }; // LIST_HEAD_INITIALIZER;
static MallocRecordList s_allocatedList = { NULL }; // LIST_HEAD_INITIALIZER;
static MutexFast s_mallocLock;

void Init()
{
	MutexFastLocker locker(s_mallocLock);

	FatLog(L"<MemCheck>: Init (costs %.3f MB)", (sizeof(s_mallocRecordInit) + sizeof(s_peakAllocCount) +
		sizeof(s_mallocRecords) + sizeof(s_freeList) + sizeof(s_allocatedList) + sizeof(s_mallocLock)) / (1024.f*1024.f));
	FatAssertNoText(s_mallocRecordInit == false);

	LIST_INIT(&s_freeList);
	LIST_INIT(&s_allocatedList);

	for (int i = MAX_MALLOC_RECORDS-1; i >= 0; --i)
	{
		LIST_INSERT_HEAD(&s_freeList, &s_mallocRecords[i], list);
		s_mallocRecords[i].ptr   = INVALID_POINTER;
		s_mallocRecords[i].file  = NULL;
		s_mallocRecords[i].line  = 0;
		s_mallocRecords[i].bytes = 0;
	}

	// MSVC crt will call delete on-exit (I don't know why)
	// Just don't catch memory alloc before Memory::Init()
	s_mallocRecordInit = true;

	s_peakAllocCount = 0;
	s_peakAllocBytes = 0;
	s_currAllocCount = 0;
	s_currAllocBytes = 0;
}

void Shutdown()
{
	FatLog(L"<MemCheck>: Shutdown - PeekCount:%u, PeekBytes:%u", s_peakAllocCount, s_peakAllocBytes);

	MutexFastLocker locker(s_mallocLock);

	FatAssertNoText(s_mallocRecordInit == true);
	s_mallocRecordInit = false;

	// remove records of lineno == -1
	if (!LIST_EMPTY(&s_allocatedList))
	{
		MallocRecord* pRecord;
		MallocRecord* pTmp;
		LIST_FOREACH_SAFE(pRecord, &s_allocatedList, list, pTmp)
		{
			if (pRecord->line == -1)
			{
				LIST_REMOVE(pRecord, list);
			}
		}
	}

	// print leak information
	if (!LIST_EMPTY(&s_allocatedList))
	{
		FatLog(L"<MemCheck>: Detect Memory Leak");

		MallocRecord* pRecord;
		LIST_FOREACH(pRecord, &s_allocatedList, list)
		{
			if (pRecord->line != -1)
			{
				FatLog(L"<MemCheck>: %ls(%u) at 0x%p, %u bytes", pRecord->file, pRecord->line, pRecord->ptr, pRecord->bytes);
			}
		}
	}
}

void* MallocDbg(size_t size, const wchar_t* file, int line)
{
	void* p = MallocInternal(size);

	if (s_mallocRecordInit)
	{
		MutexFastLocker locker(s_mallocLock);

		// find from s_freeList
		FatAssert(!LIST_EMPTY(&s_freeList), L"not enough MallocRecord, please increase MAX_MALLOC_RECORDS");

		MallocRecord* pRecord = LIST_FIRST(&s_freeList);
		LIST_REMOVE(pRecord, list);
		FatAssertNoText(pRecord->ptr == INVALID_POINTER);

		// insert to s_allocatedList
		pRecord->ptr   = p;
		pRecord->file  = file;
		pRecord->line  = line;
		pRecord->bytes = (UInt32)size;
		LIST_INSERT_HEAD(&s_allocatedList, pRecord, list);

		s_currAllocCount++;
		s_currAllocBytes += (UInt32)size;

		if (s_currAllocCount > s_peakAllocCount) s_peakAllocCount = s_currAllocCount;
		if (s_currAllocBytes > s_peakAllocBytes) s_peakAllocBytes = s_currAllocBytes;
	}

	return p;
}

void* ReallocDbg(void* p, size_t size, const wchar_t* file, int line)
{
	MallocRecord* pRecord = NULL;

	// find from s_allocatedList
	if (s_mallocRecordInit)
	{
		MutexFastLocker locker(s_mallocLock);

		LIST_FOREACH(pRecord, &s_allocatedList, list)
		{
			if (pRecord->ptr == p)
				break;
		}
		FatAssert(pRecord != NULL, L"Not in allocatedList?");
		LIST_REMOVE(pRecord, list);

		s_currAllocBytes -= pRecord->bytes;
	}

	// realloc
	void* p2 = ReallocInternal(p, size);

	// reset ptr
	if (s_mallocRecordInit)
	{
		MutexFastLocker locker(s_mallocLock);

		pRecord->ptr   = p2;
		pRecord->file  = file;
		pRecord->line  = line;
		pRecord->bytes = (UInt32)size;
		LIST_INSERT_HEAD(&s_allocatedList, pRecord, list);

		s_currAllocBytes += (UInt32)size;
		if (s_currAllocBytes > s_peakAllocBytes) s_peakAllocBytes = s_currAllocBytes;
	}

	return p2;
}

void FreeDbg(void* p)
{
	if (s_mallocRecordInit)
	{
		MutexFastLocker locker(s_mallocLock);

		// remove from s_allocatedList
		MallocRecord* pRecord;
		LIST_FOREACH(pRecord, &s_allocatedList, list)
		{
			if (pRecord->ptr == p)
				break;
		}

		FatAssert(pRecord != NULL, L"Not in allocatedList?");
		LIST_REMOVE(pRecord, list);

		// insert into s_freeList
		pRecord->ptr = INVALID_POINTER;
		LIST_INSERT_HEAD(&s_freeList, pRecord, list);

		s_currAllocCount--;
		s_currAllocBytes -= pRecord->bytes;
	}

	FreeInternal(p);
}

#else

void Init()
{
}

void Shutdown()
{
}

#endif

//
// Malloc/Realloc/Free Internal
//

#if defined(FAT_OS_WINDOWS)

void* MallocInternal(size_t size)
{
	void* p = _aligned_malloc(size, 16);
	FatAssert(p != NULL, L"Malloc failed");
	return p;
}

void* ReallocInternal(void* p, size_t size)
{
	p = _aligned_realloc(p, size, 16);
	FatAssert(p != NULL, L"Realloc failed");
	return p;
}

void FreeInternal(void* p)
{
	_aligned_free(p);
}

#else

void* MallocInternal(size_t size)
{
	void* p;
	FatIfBuildAssertion(int ret =) posix_memalign(&p, 16, size);
	FatAssert(ret == 0, L"Malloc failed");
	return p;
}

void* ReallocInternal(void* p, size_t size)
{
	// TODO
	return NULL;
}

void FreeInternal(void* p)
{
	free(p);
}

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
