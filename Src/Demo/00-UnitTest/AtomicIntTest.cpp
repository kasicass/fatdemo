#include "FatFramework.h"

#if defined(FAT_ENABLE_UNITTEST)

using namespace Fat;

TEST_DECLARE(TestAtomicInt)
{
	AtomicInt value(12);

	// Comparison operators
	FatTest(value == 12);
	FatTest(value != 21);

	// Increment
	FatTest(++value == 13);
	FatTest(value++ == 13);
	FatTest(value == 14);

	// Decrement
	FatTest(--value == 13);
	FatTest(value-- == 13);
	FatTest(value == 12);

	// Exchange
	value = 21;
	FatTest(value == 21);

	// CompareExchange
	FatTest(Atomic::CompareExchange(value, 21, 12) == 21);
	FatTest(value == 12);
	FatTest(Atomic::CompareExchange(value, 21, 0) == 12);
	FatTest(value == 12);

	// Operators
	FatTest(value >= 12);
	FatTest(value > 11);
	FatTest(value == 12);
	FatTest(value <= 12);
	FatTest(value < 13);
	FatTest(value != 135113);
}

#endif
