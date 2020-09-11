#include "FatFramework.h"
#include "Kernel/Common/Queue.h"

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
static MallocRecord s_mallocRecords[MAX_MALLOC_RECORDS];
static MallocRecordList s_freeList = { NULL }; // LIST_HEAD_INITIALIZER;
static MallocRecordList s_allocatedList = { NULL }; // LIST_HEAD_INITIALIZER;
static MutexFast s_mallocLock;

#define CheckMemLeakInitialized() FatAssertNoText(s_mallocRecordInit)

void Init()
{
	MutexFastLocker locker(s_mallocLock);

	FatLog(L"<MemCheck>: Init");
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

	s_mallocRecordInit = true;
}

void Shutdown()
{
	CheckMemLeakInitialized();
	FatLog(L"<MemCheck>: Shutdown");

	MutexFastLocker locker(s_mallocLock);

	FatAssertNoText(s_mallocRecordInit == true);
	s_mallocRecordInit = false;

	if (!LIST_EMPTY(&s_allocatedList))
	{
		FatLog(L"<MemCheck>: Detect Memory Leak");

		MallocRecord* pRecord;
		LIST_FOREACH(pRecord, &s_allocatedList, list)
		{
			FatLog(L"<MemCheck>: %ls(%u) at 0x%p, %u bytes", pRecord->file, pRecord->line, pRecord->ptr, pRecord->bytes);
		}
	}
}

void* MallocDbg(UInt32 size, const wchar_t* file, int line)
{
	CheckMemLeakInitialized();

	void* p = MallocInternal(size);

	{
		MutexFastLocker locker(s_mallocLock);

		// find from s_freeList
		FatAssert(!LIST_EMPTY(&s_freeList), L"not enough MallocRecord, please increase MAX_MALLOC_RECORDS");

		MallocRecord* pRecord = LIST_FIRST(&s_freeList);
		LIST_REMOVE(pRecord, list);
		FatAssertNoText(pRecord->ptr == INVALID_POINTER);

		// insert to s_allocatedList
		pRecord->ptr = p;
		pRecord->file = file;
		pRecord->line = line;
		pRecord->bytes = size;
		LIST_INSERT_HEAD(&s_allocatedList, pRecord, list);
	}

	return p;
}

void* ReallocDbg(void* p, UInt32 size, const wchar_t* file, int line)
{
	CheckMemLeakInitialized();

	MallocRecord* pRecord;

	// find from s_allocatedList
	{
		MutexFastLocker locker(s_mallocLock);

		LIST_FOREACH(pRecord, &s_allocatedList, list)
		{
			if (pRecord->ptr == p)
				break;
		}
		FatAssert(pRecord != NULL, L"Not in allocatedList?");
		LIST_REMOVE(pRecord, list);
	}

	// realloc
	void* p2 = ReallocInertnal(p, size);

	// reset ptr
	{
		MutexFastLocker locker(s_mallocLock);

		pRecord->ptr   = p2;
		pRecord->file  = file;
		pRecord->line  = line;
		pRecord->bytes = size;
		LIST_INSERT_HEAD(&s_allocatedList, pRecord, list);
	}

	return p2;
}

void FreeDbg(void* p)
{
	CheckMemLeakInitialized();

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
		LIST_INSERT_HEAD(&s_freeList, pRecord, list);
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

void* MallocInternal(UInt32 size)
{
	void* p = _aligned_malloc(size, 16);
	FatAssert(p != NULL, L"Malloc failed");
	return p;
}

void* ReallocInertnal(void* p, UInt32 size)
{
	p = _aligned_realloc(p, size, 16);
	FatAssert(p != NULL, L"Realloc failed");
	return p;
}

void FreeInternal(void* p)
{
	free(p);
}

#else

void* MallocInternal(UInt32 size)
{
	void* p;
	FatIfBuildAssertion(int ret =) posix_memalign(&p, 16, size);
	FatAssert(ret == 0, L"Malloc failed");
	return p;
}

void* ReallocInertnal(void* p, UInt32 size)
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
