#include "FatFramework.h"
#include "FakeDevice.h"

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
