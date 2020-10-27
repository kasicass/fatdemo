#include "FatFramework/FatFramework.h"
#include "Demo/00-UnitTest/Driver/Server/FakeDevice.h"

#if FAT_ENABLE_UNITTEST

FakeDevice* theFakeDevice = NULL;

FakeDevice::FakeDevice()
{
}

FakeDevice::~FakeDevice()
{
}

void FakeDevice::ForceResetNeeded()
{
	ResetNeeded();
}

#endif
