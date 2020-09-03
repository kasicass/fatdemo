#pragma once

namespace Fat {

typedef std::atomic<Int32> AtomicInt;

namespace Atomic {
	Int32 CompareExchange(AtomicInt& atom, Int32 comperand, Int32 exchange);
}


#if 0

class AtomicInt
{
public:
	AtomicInt(Int32 value);

	Int32 operator++(Int32);
	Int32 operator--(Int32);
	Int32 operator++();
	Int32 operator--();

	void operator=(Int32 exchange);
	Int32 CompareExchange(Int32 comperand, Int32 exchange);

	Int32 GetValue() const;
	operator Int32() const;

private:
	volatile Int32 value_;
};

#endif

}
