#include "FatFramework.h"

class UnitTestApp : public Fat::Application
{
public:
	virtual void Run() override;
};

UnitTestApp unitTestApp;
Fat::IApplication* Fat::theApp = &unitTestApp;

void UnitTestApp::Run()
{
	printf("UnitTestApp::Run()\n");
}
