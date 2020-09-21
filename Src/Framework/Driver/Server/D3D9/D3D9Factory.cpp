#include "FatFramework.h"

#if defined(FAT_OS_WINDOWS)

namespace Fat {

static D3D9Factory myD3D9Factory;
IServerFactory* theD3D9Factory = &myD3D9Factory;

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

		default:
			FatAssertUnreachableCode();
			return NULL;
		}
	}
}

}

#endif
