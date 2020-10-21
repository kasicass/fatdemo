#include "FatFramework.h"

#if defined(FAT_OS_WINDOWS)

namespace Fat {

void D3D9RenderTargetState::Build(const ICommandData* pDataIn, const IServerDevice* pServerDevice)
{
	const RenderTargetStateData* pData = (const RenderTargetStateData*)pDataIn;
	Float4 color = pData->GetColor();
	color_ = D3DCOLOR_COLORVALUE(color.x, color.y, color.z, color.w);
}

void D3D9RenderTargetState::Dispatch(const ICommandData* pDataIn, IServerDevice* pServerDevice)
{
	D3D9Device* pDevice = (D3D9Device*)pServerDevice;

	// Update shadow. Should be done in the server generic code rather than per state.
	pDevice->GetStateShadow().SetState(EStateType::eRenderTarget, this, pDataIn);

	// Forward current render target width/height to the device
	const RenderTargetStateData* pData = (const RenderTargetStateData*)pDataIn;
	pDevice->SetDeviceWidth(pData->GetWidth());
	pDevice->SetDeviceHeight(pData->GetHeight());

	// Clear this render target
	pDevice->BeginScene();
	D3D9Call(pDevice->GetD3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color_, 1, 0));
}

}

#endif
