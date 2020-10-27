#include "FatFramework/FatFramework.h"
#include "Demo/00-UnitTest/Driver/Server/FakeFactory.h"
#include "Demo/00-UnitTest/Driver/Server/FakeDevice.h"
#include "Demo/00-UnitTest/Driver/Server/Command/FakeServerCommand.h"
#include "Demo/00-UnitTest/Driver/Server/State/FakeServerState.h"

#if FAT_ENABLE_UNITTEST

using namespace Fat;

static FakeFactory myFakeFactory;
IServerFactory* GDriverFakeFactory = &myFakeFactory;

void FakeFactory::Init()
{
	FatLog(L"<FakeFactory>: Init");
	RegisterMe(EGraphicAPI::eUnitTest);
}

void FakeFactory::Shutdown()
{
	FatLog(L"<FakeFactory>: Shutdown");
}

IServerObject* FakeFactory::Instantiate(EFactoryObject::EValue value)
{
	switch (value)
	{
	case EFactoryObject::eDevice:
		theFakeDevice = FatNew(FakeDevice);
		return theFakeDevice;

	case EFactoryObject::eUnitTestCommand:
		return FatNew(FakeServerCommand);

	case EFactoryObject::eUnitTestCheckCommand:
		return FatNew(FakeCheckServerCommand);

	case EFactoryObject::eRenderTargetState:
		return FatNew(FakeRenderTargetServerState);

	case EFactoryObject::eVertexBufferState:
	case EFactoryObject::eTopologyState:
	case EFactoryObject::eMaterialState:
	case EFactoryObject::eLightState:
	case EFactoryObject::eCameraState:
	case EFactoryObject::eTransformState:
		return FatNew(FakeDefaultServerState);

	default:
		FatAssertUnreachableCode();
		return NULL;
	}
}

#endif
