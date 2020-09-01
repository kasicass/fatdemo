#include "FatFramework.h"
using namespace Fat;

class UnitTestApp : public Application
{
public:
	virtual void Run() override;
};

UnitTestApp unitTestApp;
IApplication* Fat::theApp = &unitTestApp;

void UnitTestApp::Run()
{
#if defined(FAT_ENABLE_UNITTEST)
	theUnitTestMgr->Run();
#else
	FatLog("FAT_ENABLE_UNITTEST is off");
#endif
}
