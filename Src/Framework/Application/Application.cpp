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
	InitMemoryCheck();
	thePerfCounter->Init();
}

void Application::Shutdown()
{
	thePerfCounter->Shutdown();
	FatLog(L"<App>: Shutdown");
	// Registration::CallDestroy();
}

}

using namespace Fat;

int main(int argc, char *argv[])
{
	theApp->Init();
	theApp->Run();
	theApp->Shutdown();
	return 0;
}

