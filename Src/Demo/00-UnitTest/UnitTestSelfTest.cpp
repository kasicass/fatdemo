#include "FatFramework.h"

#if defined(FAT_ENABLE_UNITTEST)

TEST_DECLARE(UnitTestSelfTest)
{
	FatTest(true);

	FatTest(0 == 0);
	FatTest(0.f == 0.f);
	FatTest(0 != 1);
	FatTest(0.f != 1.f);
	FatTest(0.f == 1.f);

	FatAssert(true, L"Internal unit test assertion");
	FatTestAssert(FatAssert(false, L"Internal unit test assertion"));

	FatValidate(true, L"Internal unit test validation");
	FatTestAssert(FatValidate(false, L"Internal unit test validation"));
}

#endif
