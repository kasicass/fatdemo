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
	Registration::CallInitialize();
	printf("Application::Init()\n");
}

void Application::Shutdown()
{
	printf("Application::Shutdown()\n");
	Registration::CallDestroy();
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
