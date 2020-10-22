#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"

#if defined(FAT_OS_WINDOWS)

#include "FatFramework/Kernel/Common/Time.h"
#include "FatFramework/Driver/Server/Interface/IServerDevice.h"
#include "FatFramework/Driver/Server/D3D9/D3D9Common.h"
#include "FatFramework/Driver/Server/D3D9/D3D9Reset.h"
#include "FatFramework/Driver/Server/D3D9/D3D9StateShadow.h"
#include "FatFramework/Driver/Server/D3D9/D3D9GPUTimer.h"

namespace Fat {

class D3D9Device : public IServerDevice, protected ID3D9ResetWarner
{
public:
	D3D9Device();
	virtual ~D3D9Device();

	IDirect3D9* GetD3DObject() const;
	IDirect3DDevice9* GetD3DDevice() const;

	D3D9StateShadow& GetStateShadow();

	UInt32 GetDeviceWidth() const;
	UInt32 GetDeviceHeight() const;

	void SetDeviceWidth(UInt32 width);
	void SetDeviceHeight(UInt32 height);

	void BeginScene();
	void EndScene();

	void ActivateLights(UInt32 lightCount);
	UInt32 GetActiveLightCount() const;

	UInt32 GetMaxActiveLights() const;
	UInt32 GetMaxPrimitiveCount() const;
	UInt32 GetMaxVertexIndex() const;

protected:
	// IServerDevice interface override
	virtual Bool Reset() override;

private:
	void ResetActiveLightCount();

	// Display overlayed text: statistics, controls
	void OverlayText();

private:
	IDirect3D9Ptr pD3D_;
	IDirect3DDevice9Ptr pD3DDevice_;

	D3D9StateShadow stateShadow_;
	// D3D9TextRenderer textRenderer_;
	D3D9GPUTimer gpuTimer_;
	FpsCounter gpuFpsCounter_;
	
	HWND hDeviceWnd_;
	UInt32 deviceWidth_;
	UInt32 deviceHeight_;

	Bool insideBeginEndScene_;
	UInt32 activeLightCount_;   // number of currently active lights
	UInt32 maxActiveLights_;    // Caps: maximum light count
	UInt32 maxPrimitiveCount_;  // Caps: maximum primitive count per draw
	UInt32 maxVertexIndex_;     // Caps: maximum vertex index
};
extern D3D9Device* theD3D9Device;

}

#endif
