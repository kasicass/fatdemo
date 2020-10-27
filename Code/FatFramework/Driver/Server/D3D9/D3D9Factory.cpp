#include "FatFramework/Driver/Server/D3D9/D3D9Factory.h"
#include "FatFramework/Kernel/Common/Log.h"
#include "FatFramework/Kernel/Common/Memory.h"
#include "FatFramework/Driver/Server/D3D9/D3D9Device.h"
#include "FatFramework/Driver/Server/D3D9/Command/D3D9SwapCommand.h"
#include "FatFramework/Driver/Server/D3D9/State/D3D9RenderTargetState.h"

#if defined(FAT_OS_WINDOWS)

namespace Fat {

static D3D9Factory myD3D9Factory;
IServerFactory* GDriverD3D9Factory = &myD3D9Factory;

void D3D9Factory::Init()
{
	FatLog(L"<Driver>: D3D9Factory Init");
	RegisterMe(EGraphicAPI::eD3D9);
}

void D3D9Factory::Shutdown()
{
	FatLog(L"<Driver>: D3D9Factory Shutdown");
}

IServerObject* D3D9Factory::Instantiate(EFactoryObject::EValue value)
{
	switch (value)
	{
	case EFactoryObject::eDevice:
		return FatNew(D3D9Device);

	case EFactoryObject::eRenderTargetState:
		return FatNew(D3D9RenderTargetState);

	case EFactoryObject::eVertexBufferState:
	case EFactoryObject::eTopologyState:
	case EFactoryObject::eMaterialState:
	case EFactoryObject::eLightState:
	case EFactoryObject::eCameraState:
	case EFactoryObject::eTransformState:
		// TODO
		return NULL;

	case EFactoryObject::eSwapCommand:
		return FatNew(D3D9SwapCommand);

	default:
		FatAssertUnreachableCode();
		return NULL;
	}
}

}

#endif
