#include "FatFramework.h"

using namespace Fat;

class HelloWndApp : public Application
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;
};
FAT_APP_DEFINE(HelloWndApp)

void HelloWndApp::Init()
{
	Application::Init();

	theFactorySelector->SelectorFactory(EGraphicAPI::eD3D9);
}

void HelloWndApp::Shutdown()
{
	Application::Shutdown();
}
