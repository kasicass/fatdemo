#include "FatFramework.h"

namespace Fat {

namespace Interlocked {

#if defined(FAT_OS_WINDOWS)

Int32 Increment(volatile Int32* pVal)
{
	return (Int32)InterlockedIncrement((volatile long*)pVal);
}

Int32 Decrement(volatile Int32* pVal)
{
	return (Int32)InterlockedDecrement((volatile long*)pVal);
}

Int32 Add(volatile Int32* pVal, Int32 value)
{
	return (Int32)InterlockedExchangeAdd((volatile long*)pVal, (long)value);;
}

Int32 Exchange(volatile Int32* pVal, Int32 exchange)
{
	return (Int32)InterlockedExchange((volatile long*)pVal, (long)exchange);
}

Int32 CompareExchange(volatile Int32* pVal, Int32 exchange, Int32 comperand)
{
	return (Int32)InterlockedCompareExchange((volatile long*)pVal, (long)exchange, (long)comperand);
}

#else

Int32 Increment(volatile Int32* pVal)
{
	Int32 r = Add(pVal, 1);
	return (r + 1); // add, since we get the original value back
}

Int32 Decrement(volatile Int32* pVal)
{
	Int32 r = Add(pVal, -1);
	return (r - 1); // substract, since we get the original value back
}

Int32 Add(volatile Int32* pVal, Int32 value)
{
	Int32 r;
	__asm__ __volatile__ (
		"lock ; xaddl %0, (%1) \n\t"
		: "=r" (r)
		: "r" (pVal), "0" (value)
		: "memory"
	);
	return r;
}

Int32 Exchange(volatile Int32* pVal, Int32 exchange)
{
	Int32 r;
	__asm__ __volatile__ (
		"lock ; xchgl %2, (%1) \n\t"
		: "=a" (r)
		: "r" (pVal), "r" (exchange)
		: "memory"
	);
	return r;
}

Int32 CompareExchange(volatile Int32* pVal, Int32 exchange, Int32 comperand)
{
	Int32 r;
	__asm__ __volatile__ (
		"lock ; cmpxchgl %2, (%1) \n\t"
		: "=a" (r)
		: "r" (pVal), "r" (exchange), "0" (comperand)
		: "memory"
	);
	return r;
}

#endif

}

AtomicInt::AtomicInt(Int32 value) :
	value_(value)
{
}

Int32 AtomicInt::operator++(Int32)
{
	return Interlocked::Add(&value_, 1);
}

Int32 AtomicInt::operator--(Int32)
{
	return Interlocked::Add(&value_, -1);
}

Int32 AtomicInt::operator++()
{
	return Interlocked::Increment(&value_);
}

Int32 AtomicInt::operator--()
{
	return Interlocked::Decrement(&value_);
}

void AtomicInt::operator=(Int32 exchange)
{
	Interlocked::Exchange(&value_, exchange);
}

Int32 AtomicInt::CompareExchange(Int32 comperand, Int32 exchange)
{
	return Interlocked::CompareExchange(&value_, exchange, comperand);
}

Int32 AtomicInt::GetValue() const
{
	return value_;
}

AtomicInt::operator Int32() const
{
	return value_;
}

}
