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
	printf("Application::Init()\n");
}

void Application::Shutdown()
{
	printf("Application::Shutdown()\n");
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
