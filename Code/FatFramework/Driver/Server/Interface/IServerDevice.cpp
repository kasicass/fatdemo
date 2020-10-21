#include "FatFramework/Driver/Server/Interface/IServerDevice.h"
#include "FatFramework/Kernel/Common/Log.h"

namespace Fat {

IServerDevice::IServerDevice() :
	resetNeeded_(0)
{
}

IServerDevice::~IServerDevice()
{
}

void IServerDevice::ResetNeeded()
{
	resetNeeded_ = 1;
}

Bool IServerDevice::IsResetNeeded() const
{
	return (resetNeeded_ == 1);
}

Bool IServerDevice::Reset()
{
	FatAssert(IsResetNeeded(), L"Reset is not required");

	Reseted();

	return true;
}

void IServerDevice::Reseted()
{
	FatAssert(IsResetNeeded(), L"Reset was not required");

	resetNeeded_ = 0;

	FatLog(L"<Driver>: ServerDevice reseted");
}

}
