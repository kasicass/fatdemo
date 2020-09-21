#include "FatFramework.h"

namespace Fat {

Application::Application()
{
	pMainWnd_ = NULL;
}

Application::~Application()
{
}

void Application::InitWidget()
{
	FatLog(L"<App>: InitWidget");
	pMainWnd_ = CreateWidget(GetTitle(), 1024, 768);
}

void Application::ShutdownWidget()
{
	FatLog(L"<App>: ShutdownWidget");
	pMainWnd_ = NULL;
}

void Application::Init()
{
	// Registration::CallInitialize();

	FatLog(L"<App>: Init");

	// Kernel
	Memory::Init();
	thePerfCounter->Init();
	Thread::InitMainThread();

	// Widget
	InitWidget();

	// Driver
	StateCache::Init();
	theDriverStats->Init();
	theFactorySelector->Init();
	theD3D9Factory->Init();
}

void Application::Shutdown()
{
	// Driver
	theD3D9Factory->Shutdown();
	theFactorySelector->Shutdown();
	theDriverStats->Shutdown();
	StateCache::Shutdown();

	// Shutdown
	ShutdownWidget();

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

void Application::Draw()
{
}

const wchar_t* Application::GetTitle() const
{
	return L"FatDemo";
}

IWidgetPtr Application::GetWidget() const
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
	theApp->Init();
	theApp->Run();
	theApp->Shutdown();
	return 0;
}

#endif
