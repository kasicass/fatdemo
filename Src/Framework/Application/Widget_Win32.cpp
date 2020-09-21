#include "FatFramework.h"

#if defined(FAT_OS_WINDOWS)

#define GAME_WINDOW_CLASSNAME L"FATDEMO-WNDCLASS"

namespace Fat {

WidgetWin32::WidgetWin32(const wchar_t* title, UInt32 width, UInt32 height)
{
	hWnd_   = NULL;
	width_  = width;
	height_ = height;
	title_  = title;
	
	FatIfBuildAssertion(bool ok =) InitWindowClass();
	FatAssertNoText(ok);

	FatIfBuildAssertion(ok = ) InitWindow();
	FatAssertNoText(ok);
}

WidgetWin32::~WidgetWin32()
{
	ShutdownWindowClass();
}

void* WidgetWin32::GetHandle() const
{
	return (void*)hWnd_;
}

Bool WidgetWin32::IsActive() const
{
	return active_;
}

void WidgetWin32::OnActive(Bool value)
{
	// FatLog(L"<App>: OnActive = %d", value);
	active_ = value;
}

bool WidgetWin32::InitWindowClass()
{
	WNDCLASS wc;

	memset(&wc, 0, sizeof(wc));

	wc.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc   = (WNDPROC)WidgetWin32::WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = GetModuleHandle(0);
	wc.hIcon         = NULL;
	wc.hCursor       = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = GAME_WINDOW_CLASSNAME;

	if (!RegisterClass(&wc))
	{
		return false;
	}

	return true;
}

void WidgetWin32::ShutdownWindowClass()
{
	UnregisterClass(GAME_WINDOW_CLASSNAME, GetModuleHandle(0));
}

bool WidgetWin32::InitWindow()
{
	UInt32 width  = width_;
	UInt32 height = height_;

	DWORD style, exstyle;
	int x, y, w, h;

	if (width < 640) width = 640;
	if (height < 480) height = 480;

	DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	// TODO: Do not allow the user to resize the window
	windowStyle &= ~WS_MAXIMIZEBOX;
	windowStyle &= ~WS_THICKFRAME;

	exstyle = WS_EX_APPWINDOW;
	style = windowStyle;
	x = (::GetSystemMetrics(SM_CXFULLSCREEN) - width) / 2;
	y = (::GetSystemMetrics(SM_CYFULLSCREEN) - height) / 2;
	w = ::GetSystemMetrics(SM_CXFIXEDFRAME) * 2 + width;
	h = ::GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + height;

	width_  = w;
	height_ = h;

	hWnd_ = ::CreateWindowEx(exstyle, GAME_WINDOW_CLASSNAME, title_, style, x, y, w, h, NULL, NULL, GetModuleHandle(0), this);

	if (!hWnd_)
		return false;

	::ShowWindow(hWnd_, SW_SHOWNORMAL);
	::SetFocus(hWnd_);
	::SetForegroundWindow(hWnd_);

	return true;
}

LRESULT CALLBACK WidgetWin32::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WidgetWin32* This = NULL;

	switch (msg)
	{
	case WM_CREATE:
		This = (WidgetWin32*)(((CREATESTRUCT*)lParam)->lpCreateParams);
		break;

	case WM_CLOSE:
		::PostQuitMessage(0);
		return 0;

	case WM_SETFOCUS:
		break;

	case WM_KILLFOCUS:
		break;

	case WM_ACTIVATEAPP:
	{
		This->OnActive(LOWORD(wParam) != WA_INACTIVE);
		return 0L;
	}
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

}

#endif
