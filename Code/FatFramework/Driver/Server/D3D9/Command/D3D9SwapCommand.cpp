#include "FatFramework/FatFramework.h"

#if defined(FAT_OS_WINDOWS)

namespace Fat {

void D3D9SwapCommand::Build(const ICommandData* pDataIn, const IServerDevice* pServerDevice)
{
}

void D3D9SwapCommand::Dispatch(const ICommandData* pDataIn, IServerDevice* pServerDevice)
{
	D3D9Device* pDevice = (D3D9Device*)pServerDevice;
	pDevice->EndScene();
	D3D9CallHandleLost(pDevice, Present(NULL, NULL, NULL, NULL));
}

}

#endif
