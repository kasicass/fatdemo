#include "FatFramework.h"
using namespace Fat;

class UnitTestApp : public Application
{
public:
	virtual void Run() override;
};
FAT_APP_DEFINE(UnitTestApp)

void UnitTestApp::Run()
{
#if defined(FAT_ENABLE_UNITTEST)
	theUnitTestMgr->Run();
#else
	FatLog(L"FAT_ENABLE_UNITTEST is off");
#endif
}

