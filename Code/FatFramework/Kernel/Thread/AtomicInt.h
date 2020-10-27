#pragma once

#include "FatFramework/Kernel/Common/Types.h"
#include <atomic>

namespace Fat {

typedef std::atomic<SInt32> AtomicInt;

namespace Atomic {
	SInt32 CompareExchange(AtomicInt& atom, SInt32 comperand, SInt32 exchange);
}


#if 0

class AtomicInt
{
public:
	AtomicInt(SInt32 value);

	SInt32 operator++(SInt32);
	SInt32 operator--(SInt32);
	SInt32 operator++();
	SInt32 operator--();

	void operator=(SInt32 exchange);
	SInt32 CompareExchange(SInt32 comperand, SInt32 exchange);

	SInt32 GetValue() const;
	operator SInt32() const;

private:
	volatile SInt32 value_;
};

#endif

}
