#include "FatFramework/FatFramework.h"

#if FAT_ENABLE_UNITTEST

using namespace Fat;

TEST_DECLARE(TestInterlocked)
{
	bool ok;

	{
		SInt32 r;
		volatile SInt32 v = 0;
		r = Interlocked::Increment32(&v);
		FatTest(1 == v);
		FatTest(1 == r);

		r = Interlocked::Increment32(&v);
		FatTest(2 == v);
		FatTest(2 == r);

		r = Interlocked::Decrement32(&v);
		FatTest(1 == v);

		r = Interlocked::Add32(&v, 3);
		FatTest(4 == v);
		FatTest(1 == r);

		r = Interlocked::Add32(&v, -2);
		FatTest(2 == v);
		FatTest(4 == r);

		ok = Interlocked::CompareExchange32(&v, 10, 2);
		FatTest(10 == v);
		FatTest(ok);

		ok = Interlocked::CompareExchange32(&v, 5, 20);
		FatTest(10 == v);
		FatTest(!ok);
	}

	{
		SInt64 r;
		volatile SInt64 v = 0;
		r = Interlocked::Increment64(&v);
		FatTest(1 == v);
		FatTest(1 == r);

		r = Interlocked::Increment64(&v);
		FatTest(2 == v);
		FatTest(2 == r);

		r = Interlocked::Decrement64(&v);
		FatTest(1 == v);

		r = Interlocked::Add64(&v, 3);
		FatTest(4 == v);
		FatTest(1 == r);

		r = Interlocked::Add64(&v, -2);
		FatTest(2 == v);
		FatTest(4 == r);

		ok = Interlocked::CompareExchange64(&v, 10, 2);
		FatTest(10 == v);
		FatTest(ok);

		ok = Interlocked::CompareExchange64(&v, 5, 20);
		FatTest(10 == v);
		FatTest(!ok);
	}
}

#endif
