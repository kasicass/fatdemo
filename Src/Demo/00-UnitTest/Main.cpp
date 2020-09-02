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
	TEST_REGISTER(UnitTestSelfTest);
	TEST_REGISTER(TestTime);
	TEST_REGISTER(TestFpsCounter);
//	TEST_REGISTER(TestAtomicInt);
	theUnitTestMgr->Run();
#else
	FatLog(L"FAT_ENABLE_UNITTEST is off");
#endif
}
