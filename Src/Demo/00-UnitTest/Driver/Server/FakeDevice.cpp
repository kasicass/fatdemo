#include "FatFramework.h"
#include "FakeDevice.h"

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
