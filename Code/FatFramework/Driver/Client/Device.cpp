#include "FatFramework/Driver/Client/Device.h"
#include "FatFramework/Driver/Server/Interface/IServerDevice.h"

namespace Fat {

Device::Device() :
	IObject(EFactoryObject::eDevice)
{
}

Device::~Device()
{
}

bool Device::Reset()
{
	FatAssert(IsResetNeeded() == true, L"Device reset isn't required");
	return GetServerDevice()->Reset();
}

bool Device::IsResetNeeded() const
{
	return GetServerDevice()->IsResetNeeded();
}

IServerDevice* Device::GetServerDevice() const
{
	return (IServerDevice*)GetServerObject();
}

}
