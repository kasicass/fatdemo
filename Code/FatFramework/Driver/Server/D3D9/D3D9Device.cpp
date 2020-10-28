#include "FatFramework/Driver/Server/D3D9/D3D9Device.h"
#include "FatFramework/Kernel/Common/Log.h"
#include "FatFramework/Application/IApplication.h"
#include "FatFramework/Application/IWidget.h"
#include "FatFramework/Driver/Client/DriverStatistics.h"

#if FAT_OS_WINDOWS

namespace Fat {

D3D9Device* theD3D9Device = NULL;

D3D9Device::D3D9Device()
{
	FatLog(L"<Driver>: Instancing D3D9 Server");

	FatAssertNoText(theD3D9Device == NULL);
	theD3D9Device = this;

	hDeviceWnd_          = NULL;
	deviceWidth_         = 0;
	deviceHeight_        = 0;

	insideBeginEndScene_ = false;
	activeLightCount_    = 0;
	maxActiveLights_     = 0;
	maxPrimitiveCount_   = 0;
	maxVertexIndex_      = 0;

	// D3D automatically increment references, we must explicitly release it because
	// the smart pointer stores the real reference
	pD3D_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D_.IsValid())
	{
		FatLog(L"<Driver>: Direct3DCreate9() fails");
		fat_debugbreak();
	}
	pD3D_->Release();

	// Create D3D9 device
	hDeviceWnd_ = (HWND)GApp->GetMainWnd()->GetHandle();
	FatAssert(hDeviceWnd_, L"Platform-specific window handle must not be NULL");

	RECT clientRect;
	FatValidate(::GetClientRect(hDeviceWnd_, &clientRect) == TRUE, L"GetClientRect() failed");

	deviceWidth_  = clientRect.right - clientRect.left;
	deviceHeight_ = clientRect.bottom - clientRect.top;

	D3D9PresentParameter presentParam(pD3D_.Get(), deviceWidth_, deviceHeight_, hDeviceWnd_);
	UInt32 creationFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING; // TODO: D3DCREATE_MULTITHREADED needed?
	FatValidate(SUCCEEDED(pD3D_->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hDeviceWnd_,
		creationFlags, presentParam, GetD3D9SmartModifier(pD3DDevice_))), L"D3D9 Device creation failed");
	FatLog(L"<Driver>: D3D9 device initialized");

	// Call the reset function in order to always use the same initialization path
	ResetNeeded();

	// Initialize internal caps based on D3D9
	D3DCAPS9 caps;
	D3D9Call(pD3DDevice_->GetDeviceCaps(&caps));

	maxActiveLights_   = caps.MaxActiveLights;
	maxPrimitiveCount_ = caps.MaxPrimitiveCount;
	maxVertexIndex_    = caps.MaxVertexIndex;
}

D3D9Device::~D3D9Device()
{
	FatLog(L"<Driver>: Releasing D3D9 Server");

	FatAssertNoText(theD3D9Device != NULL);
	theD3D9Device = NULL;

	pD3DDevice_ = NULL;
	pD3D_ = NULL;
}

IDirect3D9* D3D9Device::GetD3DObject() const
{
	return pD3D_.Get();
}

IDirect3DDevice9* D3D9Device::GetD3DDevice() const
{
	return pD3DDevice_.Get();
}

D3D9StateShadow& D3D9Device::GetStateShadow()
{
	return stateShadow_;
}

UInt32 D3D9Device::GetDeviceWidth() const
{
	return deviceWidth_;
}

UInt32 D3D9Device::GetDeviceHeight() const
{
	return deviceHeight_;
}

void D3D9Device::SetDeviceWidth(UInt32 width)
{
	if (deviceWidth_ != width)
	{
		FatLog(L"<Driver>: SetDeviceWidth, old:%u, new:%u", deviceWidth_, width);
		deviceWidth_ = width;
		ResetNeeded();
	}
}

void D3D9Device::SetDeviceHeight(UInt32 height)
{
	if (deviceHeight_ != height)
	{
		FatLog(L"<Driver>: SetDeviceHeight, old:%u, new:%u", deviceHeight_, height);
		deviceHeight_ = height;
		ResetNeeded();
	}
}

void D3D9Device::ActivateLights(UInt32 lightCount)
{
	// Enable required light count
	for (UInt32 i = activeLightCount_; i < lightCount; ++i)
	{
		pD3DDevice_->LightEnable(i, TRUE);
	}

	// Disable all other active lights
	for (UInt32 i = lightCount; i < activeLightCount_; ++i)
	{
		pD3DDevice_->LightEnable(i, FALSE);
	}

	activeLightCount_ = lightCount;
}

void D3D9Device::ResetActiveLightCount()
{
	activeLightCount_ = 0;
}

UInt32 D3D9Device::GetActiveLightCount() const
{
	return activeLightCount_;
}

UInt32 D3D9Device::GetMaxActiveLights() const
{
	return maxActiveLights_;
}

UInt32 D3D9Device::GetMaxPrimitiveCount() const
{
	return maxPrimitiveCount_;
}

UInt32 D3D9Device::GetMaxVertexIndex() const
{
	return maxVertexIndex_;
}

void D3D9Device::BeginScene()
{
	if (!insideBeginEndScene_)
	{
		insideBeginEndScene_ = true;

		gpuTimer_.Begin();

		D3D9Call(pD3DDevice_->BeginScene());
	}
}

void D3D9Device::EndScene()
{
	FatAssert(insideBeginEndScene_, L"Should not be called if BeginScene hasn't been called");

	OverlayText();

	gpuTimer_.End();

	// Now really end the frame
	insideBeginEndScene_ = false;

	// Release all references at the end of the frame as server objects and data might be destroyed
	stateShadow_.Release();

	D3D9Call(pD3DDevice_->EndScene());
}

bool D3D9Device::Reset()
{
	// 1. Try to see if the device could be reseted using TestCooperativeLevel
	HRESULT hr = pD3DDevice_->TestCooperativeLevel();
	switch (hr)
	{
	case D3D_OK:
		// The device is operational and the calling application can continue. We're still going to do
		// the reset because we need to close this chapter by restoring video memory.
		break;

	case D3DERR_DEVICENOTRESET:
		// If an application detects a lost device, it should pause and periodically call
		// TestCooperativeLevel until it receives a return value of D3DERR_DEVICENOTRESET. Don't return
		// but continue to the reset function
		break;

	case D3DERR_DRIVERINTERNALERROR:
	case D3DERR_DEVICELOST:
		// If the device is lost but cannot be restored at the current time, TestCooperativeLevel
		// returns the D3DERR_DEVICELOST return code.
		return false;

	default:
		FatAssert(false, D3D9GetErrorString(hr));
		return false;
	}
	
	// 2. Prepare present parameters
	D3D9PresentParameter presentParam(pD3D_.Get(), deviceWidth_, deviceHeight_, hDeviceWnd_);

	// 3. PreReset : Release all video memory resources
	SendPreReset();

	// 4. Do the actual reset
	hr = pD3DDevice_->Reset(presentParam);
	switch (hr)
	{
	case D3D_OK:
		// Reset Succeed: Restore video memory and re-init render states
		SendPostResetSucceed();

		// Initialize default states
		D3D9Call(pD3DDevice_->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE)); // Enable Z buffer
		D3D9Call(pD3DDevice_->SetRenderState(D3DRS_LIGHTING, TRUE));      // Enable lighting
		ResetActiveLightCount();
		Reseted();
		return true;

	default:
		SendPostResetFailed();

		// When IDirect3DDevice9::Reset fails, the only valid methods that can be called are
		// IDirect3DDevice9::Reset, IDirect3DDevice9::TestCooperativeLevel, and the various Release
		// member functions. Calling any other method can result in an exception.

		// Reset has failed, we can only exit and try again next frame. False is return in order to
		// inform the application that no rendering can be done.
		return false;
	}
}

void D3D9Device::OverlayText()
{

}

}

#endif
