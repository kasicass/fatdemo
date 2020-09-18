#pragma once

#if defined(FAT_OS_WINDOWS)

namespace Fat {

class D3D9Device : public IServerDevice, protected ID3D9ResetWarner
{
public:
	D3D9Device();
	virtual ~D3D9Device();

	IDirect3D9* GetD3DObject() const;
	IDirect3DDevice9* GetD3DDevice() const;

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
