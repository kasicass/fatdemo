#include "FatFramework.h"

namespace Fat {

Application::Application()
{
	pMainWnd_ = NULL;
}

Application::~Application()
{
}

void Application::InitWidget(UInt32 width, UInt32 height)
{
	FatLog(L"<App>: InitWidget");
	pMainWnd_ = CreateWidget(GetTitle(), width, height);
}

void Application::ShutdownWidget()
{
	FatLog(L"<App>: ShutdownWidget");
	FAT_SAFE_DELETE(pMainWnd_);
}

void Application::Init()
{
	// Registration::CallInitialize();

	FatLog(L"<App>: Init");

	// Kernel
	Memory::Init();
	thePerfCounter->Init();
	Thread::InitMainThread();

	// Driver
	StateCache::Init();
	theDriverStats->Init();
	theFactorySelector->Init();
#if defined(FAT_OS_WINDOWS)
	theD3D9Factory->Init();
#endif
}

void Application::Shutdown()
{
	// Driver
#if defined(FAT_OS_WINDOWS)
	theD3D9Factory->Shutdown();
#endif
	theFactorySelector->Shutdown();
	theDriverStats->Shutdown();
	StateCache::Shutdown();

	// Kernel
	Thread::ShutdownMainThread();
	thePerfCounter->Shutdown();
	Memory::Shutdown();
	FatLog(L"<App>: Shutdown");
	// Registration::CallDestroy();
}


void Application::Update()
{
}

void Application::Render()
{
}

void Application::Resize(UInt32 width, UInt32 height)
{
}

const wchar_t* Application::GetTitle() const
{
	return L"FatDemo";
}

IWidget* Application::GetMainWnd() const
{
	return pMainWnd_;
}

#if !defined(FAT_OS_ANDROID)
void Application::Run()
{
}
#endif

}


#if !defined(FAT_OS_ANDROID)

using namespace Fat;

int main(int argc, char *argv[])
{
#if defined(FAT_OS_WINDOWS)
	// Setting default awareness with the application manifest
	// https://docs.microsoft.com/en-us/windows/win32/hidpi/setting-the-default-dpi-awareness-for-a-process
	HINSTANCE hUser32 = LoadLibrary(L"user32.dll");
	if (hUser32)
	{
		typedef BOOL(WINAPI* SetProcessDPIAwareFn)(void);
		SetProcessDPIAwareFn fn = (SetProcessDPIAwareFn)GetProcAddress(hUser32, "SetProcessDPIAware");
		if (fn) fn();
		FreeLibrary(hUser32);
	}
#endif

	theApp->Init();
	theApp->Run();
	theApp->Shutdown();
	return 0;
}

#endif
