#include "FatFramework.h"
using namespace Fat;

class UnitTestApp : public Application
{
public:
	virtual void Init() override;
};
FAT_APP_DEFINE(UnitTestApp)

void UnitTestApp::Init()
{
	Application::Init();

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
