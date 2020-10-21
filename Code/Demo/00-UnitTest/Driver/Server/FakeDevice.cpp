#include "FatFramework.h"
#include "FakeDevice.h"

#if defined(FAT_ENABLE_UNITTEST)

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
