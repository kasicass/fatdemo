#include "FatFramework/FatFramework.h"

#if FAT_ENABLE_UNITTEST

using namespace Fat;

TEST_DECLARE(TestFloat3)
{
	Float3 a(2, 1, 0);

	Float3 b(0, 1, 2);
	FatTest(b.x == 0 && b.y == 1 && b.z == 2);

	Float3 c(b);
	FatTest(c.x == 0 && c.y == 1 && c.z == 2);

	Float3 d;
	d = b;
	FatTest(d.x == 0 && d.y == 1 && d.z == 2);

	FatTest(d == c);
	FatTest(d != a);
}

TEST_DECLARE(TestFloat4)
{
	Float4 a(3, 2, 1, 0);

	Float4 b(0, 1, 2, 3);
	FatTest(b.x == 0 && b.y == 1 && b.z == 2 && b.w == 3);

	Float4 c(b);
	FatTest(c.x == 0 && c.y == 1 && c.z == 2 && c.w == 3);

	Float4 d;
	d = b;
	FatTest(d.x == 0 && d.y == 1 && d.z == 2 && d.w == 3);

	FatTest(d == c);
	FatTest(d != a);
}

#endif
