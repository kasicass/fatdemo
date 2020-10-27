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

Int32 CompareExchange(AtomicInt& atom, Int32 comperand, Int32 exchange)
{
	atom.compare_exchange_weak(comperand, exchange);
	return comperand;
}

}}

#if 0

namespace Fat {

namespace Interlocked {

#if FAT_OS_WINDOWS

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

#endif
