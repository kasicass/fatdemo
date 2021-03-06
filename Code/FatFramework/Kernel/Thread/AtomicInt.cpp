#include "FatFramework/Kernel/Thread/AtomicInt.h"

namespace Fat { namespace Atomic {

// It has to do with the shared memory consistency model that the hardware implements. 
// For those hardware architectures that implement some kind of relaxed consistency model
// (e.g. release semantics), the strong operations you refer to above can have a high overhead,
// and thus experts can use the weaker forms to implement algorithms that perform well also
// on those relaxed consistency architectures.
//
// For more info, see e.g. http://www.hpl.hp.com/techreports/Compaq-DEC/WRL-95-7.pdf
//
// Chapter 12 and Appendix C in http://kernel.org/pub/linux/kernel/people/paulmck/perfbook/perfbook.html

SInt32 CompareExchange(AtomicInt& atom, SInt32 comperand, SInt32 exchange)
{
	atom.compare_exchange_weak(comperand, exchange);
	return comperand;
}

}}

#if 0

namespace Fat {

namespace Interlocked {

#if FAT_OS_WINDOWS

SInt32 Increment(volatile SInt32* pVal)
{
	return (SInt32)InterlockedIncrement((volatile long*)pVal);
}

SInt32 Decrement(volatile SInt32* pVal)
{
	return (SInt32)InterlockedDecrement((volatile long*)pVal);
}

SInt32 Add(volatile SInt32* pVal, SInt32 value)
{
	return (SInt32)InterlockedExchangeAdd((volatile long*)pVal, (long)value);;
}

SInt32 Exchange(volatile SInt32* pVal, SInt32 exchange)
{
	return (SInt32)InterlockedExchange((volatile long*)pVal, (long)exchange);
}

SInt32 CompareExchange(volatile SInt32* pVal, SInt32 exchange, SInt32 comperand)
{
	return (SInt32)InterlockedCompareExchange((volatile long*)pVal, (long)exchange, (long)comperand);
}

#else

SInt32 Add(volatile SInt32* pVal, SInt32 value)
{
	SInt32 r;
	__asm__ __volatile__ (
		"lock ; xaddl %0, (%1) \n\t"
		: "=r" (r)
		: "r" (pVal), "0" (value)
		: "memory"
	);
	return r;
}

SInt32 Increment(volatile SInt32* pVal)
{
	SInt32 r = Add(pVal, 1);
	return (r + 1); // add, since we get the original value back
}

SInt32 Decrement(volatile SInt32* pVal)
{
	SInt32 r = Add(pVal, -1);
	return (r - 1); // substract, since we get the original value back
}


SInt32 Exchange(volatile SInt32* pVal, SInt32 exchange)
{
	SInt32 r;
	__asm__ __volatile__ (
		"lock ; xchgl %2, (%1) \n\t"
		: "=a" (r)
		: "r" (pVal), "r" (exchange)
		: "memory"
	);
	return r;
}

SInt32 CompareExchange(volatile SInt32* pVal, SInt32 exchange, SInt32 comperand)
{
	SInt32 r;
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

AtomicInt::AtomicInt(SInt32 value) :
	value_(value)
{
}

SInt32 AtomicInt::operator++(SInt32)
{
	return Interlocked::Add(&value_, 1);
}

SInt32 AtomicInt::operator--(SInt32)
{
	return Interlocked::Add(&value_, -1);
}

SInt32 AtomicInt::operator++()
{
	return Interlocked::Increment(&value_);
}

SInt32 AtomicInt::operator--()
{
	return Interlocked::Decrement(&value_);
}

void AtomicInt::operator=(SInt32 exchange)
{
	Interlocked::Exchange(&value_, exchange);
}

SInt32 AtomicInt::CompareExchange(SInt32 comperand, SInt32 exchange)
{
	return Interlocked::CompareExchange(&value_, exchange, comperand);
}

SInt32 AtomicInt::GetValue() const
{
	return value_;
}

AtomicInt::operator SInt32() const
{
	return value_;
}

}

#endif
