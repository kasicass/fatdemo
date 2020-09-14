#include "FatFramework.h"

namespace Fat {

Application::Application()
{
}

Application::~Application()
{
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
	theDriverStats->Init();
	theFactorySelector->Init();
}

void Application::Shutdown()
{
	// Driver
	theDriverStats->Shutdown();
	theFactorySelector->Shutdown();

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
