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
	InitMemoryCheck();
	FatLog(L"<App>: Init");
}

void Application::Shutdown()
{
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
