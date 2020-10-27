// C# System.Threading.Interlocked
// https://docs.microsoft.com/en-us/dotnet/api/system.threading.interlocked?view=netframework-4.6
//
// sizeof(long) = 4 in win32/win64
// sizeof(long) = 4 in openbsd x86, sizeof(long) = 8 in openbsd amd64
//
// Add(), returns the new value stored at pVal.
// CompareExchange(), returns the original value in pVal.

#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/PlatformHeaders.h"
#include "FatFramework/Kernel/Common/Types.h"

namespace Fat { namespace Interlocked {

FAT_FORCE_INLINE SInt32 Increment32(volatile SInt32* pVal);
FAT_FORCE_INLINE SInt32 Decrement32(volatile SInt32* pVal);
FAT_FORCE_INLINE SInt32 Add32(volatile SInt32* pVal, SInt32 value);
FAT_FORCE_INLINE SInt32 CompareExchange32(volatile SInt32* pVal, SInt32 exchange, SInt32 comperand);

FAT_FORCE_INLINE SInt64 Increment64(volatile SInt64* pVal);
FAT_FORCE_INLINE SInt64 Decrement64(volatile SInt64* pVal);
FAT_FORCE_INLINE SInt64 Add64(volatile SInt64* pVal, SInt64 value);
FAT_FORCE_INLINE SInt64 CompareExchange64(volatile SInt64* pVal, SInt64 exchange, SInt64 comperand);

#if FAT_OS_WINDOWS

SInt32 Increment32(volatile SInt32* pVal)
{
	return (SInt32)InterlockedIncrement((volatile long*)pVal);
}

SInt32 Decrement32(volatile SInt32* pVal)
{
	return (SInt32)InterlockedDecrement((volatile long*)pVal);
}

SInt32 Add32(volatile SInt32* pVal, SInt32 value)
{
	return (SInt32)InterlockedExchangeAdd((volatile long*)pVal, (long)value);
}

SInt32 CompareExchange32(volatile SInt32* pVal, SInt32 exchange, SInt32 comperand)
{
	return (SInt32)InterlockedCompareExchange((volatile long*)pVal, (long)exchange, (long)comperand);
}

SInt64 Increment64(volatile SInt64* pVal)
{
	return InterlockedIncrement64(pVal);
}

SInt64 Decrement64(volatile SInt64* pVal)
{
	return InterlockedDecrement64(pVal);
}

SInt64 Add64(volatile SInt64* pVal, SInt64 value)
{
	return InterlockedExchangeAdd64(pVal, value);
}

SInt64 CompareExchange64(volatile SInt64* pVal, SInt64 exchange, SInt64 comperand)
{
	return InterlockedCompareExchange64(pVal, exchange, comperand);
}

#else

// TODO

#endif

}}
