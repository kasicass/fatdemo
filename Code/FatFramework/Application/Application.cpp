#include "FatFramework/FatFramework.h" // TODO

#if 0
#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/PlatformHeaders.h"
#include "FatFramework/Kernel/Common/Macros.h"
#include "FatFramework/Kernel/Common/Log.h"
#include "FatFramework/Kernel/Common/Memory.h"
#include "FatFramework/Application/Application.h"
#include "FatFramework/Application/IWidget.h"
#endif

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
	GPerfCounter->Init();
	Thread::InitMainThread();

	// Driver
	StateCache::Init();
	GDriverStats->Init();
	GDriverFactorySelector->Init();
#if FAT_OS_WINDOWS
	GDriverD3D9Factory->Init();
#endif
}

void Application::Shutdown()
{
	// Driver
#if FAT_OS_WINDOWS
	GDriverD3D9Factory->Shutdown();
#endif
	GDriverFactorySelector->Shutdown();
	GDriverStats->Shutdown();
	StateCache::Shutdown();

	// Kernel
	Thread::ShutdownMainThread();
	GPerfCounter->Shutdown();
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

#if !FAT_OS_ANDROID
void Application::Run()
{
}
#endif

}


#if !FAT_OS_ANDROID

using namespace Fat;

int main(int argc, char *argv[])
{
#if FAT_OS_WINDOWS
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

	GApp->Init();
	GApp->Run();
	GApp->Shutdown();
	return 0;
}

#endif
