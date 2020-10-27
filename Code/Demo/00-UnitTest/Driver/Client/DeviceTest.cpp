#include "FatFramework/FatFramework.h"
#include "Demo/00-UnitTest/Driver/Server/FakeDevice.h"

#if FAT_ENABLE_UNITTEST

using namespace Fat;

TEST_DECLARE(TestDeviceReset)
{
	FactoryRAIISelector raiiSelector(EGraphicAPI::eUnitTest);

	Device device;
	FatTest(device.IsResetNeeded() == false);
	FatTestAssert(device.Reset());

	theFakeDevice->ForceResetNeeded();

	FatTest(device.IsResetNeeded() == true);
	FatTest(device.Reset());
	FatTest(device.IsResetNeeded() == false);
}

#endif
