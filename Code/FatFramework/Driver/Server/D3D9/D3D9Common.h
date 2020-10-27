#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/Common/SmartPtr.h"

#if defined(FAT_OS_WINDOWS)

// https://github.com/kasicass/blog/blob/master/3dapi-d3d9/2020_08_29_d3d_debug_info.md
#if !defined(FAT_RELEASE_BUILD)
#  define D3D_DEBUG_INFO
#endif

#if defined(FAT_ENABLE_ASSERT)
#  define FAT_D3D9_API_ERROR_CHECK
#endif

// Include d3d9 SDK files
#include <d3d9.h>
#include <d3dx9.h>

#if defined(FAT_D3D9_API_ERROR_CHECK)
#include "dxerr.h"
#endif

namespace Fat {

// Define D3D9 SmartPtrs
typedef TSmartPtr<IUnknown,                    TSmartPtrCOMPolicy> IUnknownPtr;
typedef TSmartPtr<IDirect3D9,                  TSmartPtrCOMPolicy> IDirect3D9Ptr;
typedef TSmartPtr<IDirect3DDevice9,            TSmartPtrCOMPolicy> IDirect3DDevice9Ptr;
typedef TSmartPtr<IDirect3DSwapChain9,         TSmartPtrCOMPolicy> IDirect3DSwapChain9Ptr;
typedef TSmartPtr<IDirect3DVertexDeclaration9, TSmartPtrCOMPolicy> IDirect3DVertexDeclaration9Ptr;
typedef TSmartPtr<IDirect3DSurface9,           TSmartPtrCOMPolicy> IDirect3DSurface9Ptr;
typedef TSmartPtr<IDirect3DBaseTexture9,       TSmartPtrCOMPolicy> IDirect3DBaseTexture9Ptr;
typedef TSmartPtr<IDirect3DTexture9,           TSmartPtrCOMPolicy> IDirect3DTexture9Ptr;
typedef TSmartPtr<IDirect3DCubeTexture9,       TSmartPtrCOMPolicy> IDirect3DCubeTexture9Ptr;
typedef TSmartPtr<IDirect3DVolumeTexture9,     TSmartPtrCOMPolicy> IDirect3DVolumeTexture9Ptr;
typedef TSmartPtr<IDirect3DIndexBuffer9,       TSmartPtrCOMPolicy> IDirect3DIndexBuffer9Ptr;
typedef TSmartPtr<IDirect3DVertexBuffer9,      TSmartPtrCOMPolicy> IDirect3DVertexBuffer9Ptr;
typedef TSmartPtr<IDirect3DVertexShader9,      TSmartPtrCOMPolicy> IDirect3DVertexShader9Ptr;
typedef TSmartPtr<IDirect3DPixelShader9,       TSmartPtrCOMPolicy> IDirect3DPixelShader9Ptr;
typedef TSmartPtr<IDirect3DQuery9,             TSmartPtrCOMPolicy> IDirect3DQuery9Ptr;

// Define D3DX SmartPtrs
typedef TSmartPtr<ID3DXBuffer,                 TSmartPtrCOMPolicy> ID3DXBufferPtr;
typedef TSmartPtr<ID3DXConstantTable,          TSmartPtrCOMPolicy> ID3DXConstantTablePtr;
typedef TSmartPtr<ID3DXFont,                   TSmartPtrCOMPolicy> ID3DXFontPtr;

// Implement extended function call error checking
#if defined(FAT_D3D9_API_ERROR_CHECK)

#define D3D9GetErrorString(hr) DXGetErrorString(hr)

#define D3D9Call(x_function) \
	{ \
		GDriverStats->IncD3D9CallsPerFrame(); \
		HRESULT hr = x_function; \
		FatAssert(SUCCEEDED(hr), L"%s\n%ls", #x_function, D3D9GetErrorString(hr)); \
	}

#define D3D9CallHandleLost(x_device, x_function) \
	{ \
		GDriverStats->IncD3D9CallsPerFrame(); \
		HRESULT hr = x_device->GetD3DDevice()->x_function; \
		if (hr == D3DERR_DEVICELOST) \
		{ \
			x_device->ResetNeeded(); \
		} \
		else \
		{ \
			FatAssert(SUCCEEDED(hr), L"%s\n%ls", #x_function, D3D9GetErrorString(hr)); \
		} \
	}

#else

#define D3D9GetErrorString(hr) L"D3D9 Error"

#define D3D9Call(x_function) \
	{ \
		GDriverStats->IncD3D9CallsPerFrame(); \
		x_function; \
	}

#define D3D9CallHandleLost(x_device, x_function) \
	{ \
		GDriverStats->IncD3D9CallsPerFrame(); \
		HRESULT hr = x_device->GetD3DDevice()->x_function; \
		if (hr == D3DERR_DEVICELOST) { x_device->ResetNeeded(); } \
	}

#endif

// Encapsulate D3DPRESENT_PARAMETERS initialiation
class D3D9PresentParameter
{
public:
	D3D9PresentParameter(IDirect3D9* pD3D, UInt32 width, UInt32 height, HWND hWnd);

	operator D3DPRESENT_PARAMETERS*() { return &d3d9PresentParameter_; }

private:
	D3DPRESENT_PARAMETERS d3d9PresentParameter_;
};

// Implement a helper macro to initialize smart pointers when filled in a D3D9 get function
template <typename t_SmartPtr>
class D3D9SmartPtrProxy
{
public:
	typedef typename t_SmartPtr::Pointer Pointer;

	D3D9SmartPtrProxy(t_SmartPtr& p) :
		rSmartPtr_(p),
		ptr_(NULL)
	{
	}

	~D3D9SmartPtrProxy()
	{
		// Set the smart pointer to new value
		rSmartPtr_ = ptr_;

		// D3D9 automatically adds a reference to the pointer, so we need to release it
		if (ptr_ != NULL)
		{
			ptr_->Release();
		}
	}

	Pointer* GetProxyPointer() { return &ptr_; }

private:
	t_SmartPtr& rSmartPtr_;
	Pointer ptr_;
};

template <typename t_SmartPtr>
inline D3D9SmartPtrProxy<t_SmartPtr> CreateD3D9SmartPtrProxy(t_SmartPtr& rSmartPtr)
{
	return D3D9SmartPtrProxy<t_SmartPtr>(rSmartPtr);
}

}

#define GetD3D9SmartModifier(aSmartPtr) CreateD3D9SmartPtrProxy(aSmartPtr).GetProxyPointer()

#endif
