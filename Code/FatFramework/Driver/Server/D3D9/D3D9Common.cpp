#include "FatFramework/Driver/Server/D3D9/D3D9Common.h"
#include "FatFramework/Kernel/Common/Memory.h"
#include "FatFramework/Kernel/Math/Utils.h"

#if defined(FAT_OS_WINDOWS)

namespace Fat {

//
// D3D9PresentParameter
//

D3D9PresentParameter::D3D9PresentParameter(IDirect3D9* pD3D, UInt32 width, UInt32 height, HWND hWnd)
{
	MemoryZero(d3d9PresentParameter_);

	// Back buffer is of a size of minimum 1 pixel
	d3d9PresentParameter_.BackBufferWidth  = FatMax(1u, width);
	d3d9PresentParameter_.BackBufferHeight = FatMax(1u, height);
	d3d9PresentParameter_.BackBufferCount  = 1;
	d3d9PresentParameter_.BackBufferFormat = D3DFMT_X8R8G8B8;

	// Automatically build the Z buffer
	d3d9PresentParameter_.EnableAutoDepthStencil = true;
	d3d9PresentParameter_.AutoDepthStencilFormat = D3DFMT_D24X8;

	// Multi sampling
	D3DMULTISAMPLE_TYPE supportedMultisampledType[] = {
		D3DMULTISAMPLE_4_SAMPLES,
		D3DMULTISAMPLE_2_SAMPLES,
	};

	D3DMULTISAMPLE_TYPE selectedType = D3DMULTISAMPLE_NONE;
	for (int i = 0; i < FAT_ARRAY_SIZE(supportedMultisampledType); ++i)
	{
		if (D3D_OK == pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			d3d9PresentParameter_.BackBufferFormat, TRUE, supportedMultisampledType[i], NULL))
		{
			selectedType = supportedMultisampledType[i];
			break;
		}
	}

	d3d9PresentParameter_.MultiSampleType = selectedType;
	d3d9PresentParameter_.MultiSampleQuality = 0;

	// Swapping
	d3d9PresentParameter_.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3d9PresentParameter_.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3d9PresentParameter_.FullScreen_RefreshRateInHz = 0;
	d3d9PresentParameter_.Flags = 0;

	// Window
	d3d9PresentParameter_.Windowed = TRUE;
	d3d9PresentParameter_.hDeviceWindow = hWnd;
}

}

#endif
