#include "Driver/Client/Device.h"
#include "Driver/Server/Interface/IServerDevice.h"

namespace Fat {

Device::Device() :
	IObject(EFactoryObject::eDevice)
{
}

Device::~Device()
{
}

Bool Device::Reset()
{
	FatAssert(IsResetNeeded() == true, L"Device reset isn't required");
	return GetServerDevice()->Reset();
}

Bool Device::IsResetNeeded() const
{
	return GetServerDevice()->IsResetNeeded();
}

IServerDevice* Device::GetServerDevice() const
{
	return (IServerDevice*)GetServerObject();
}

}
