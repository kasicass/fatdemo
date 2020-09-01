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
	theMemMgr->Init();
	FatLog(L"Application::Init()");
}

void Application::Shutdown()
{
	FatLog(L"Application::Shutdown()");
	theMemMgr->Shutdown();
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
