#include "FatFramework.h"
#include "Driver/Server/FakeFactory.h"

using namespace Fat;

class UnitTestApp : public Application
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;
};
FAT_APP_DEFINE(UnitTestApp)

void UnitTestApp::Init()
{
	Application::Init();
	
#if defined(FAT_ENABLE_UNITTEST)
	theFakeFactory->Init();
	TEST_REGISTER(UnitTestSelfTest);
	TEST_REGISTER(TestTime);
	TEST_REGISTER(TestFpsCounter);
	TEST_REGISTER(TestAtomicInt);
	TEST_REGISTER(TestSmartPtrAssertPolicy);
	TEST_REGISTER(TestSmartPtrIntrusivePolicy);
	TEST_REGISTER(TestSmartPtrCOMPolicy);
	TEST_REGISTER(TestThreadOperators);
	TEST_REGISTER(TestThreadCreation);
	TEST_REGISTER(TestThreadMutiJoin);
	TEST_REGISTER(TestMutex);
	TEST_REGISTER(TestConditionVariable);
	TEST_REGISTER(TestDeviceReset);
	TEST_REGISTER(TestCommandLocker);
	TEST_REGISTER(TestCommandLocking);
	TEST_REGISTER(TestCommandDataIntegrity);
	TEST_REGISTER(TestCommandBufferModes);
	theUnitTestMgr->Run();
#else
	FatLog(L"FAT_ENABLE_UNITTEST is off");
#endif

	void* p = FatMalloc(10);
	*(char*)p = 1;
	FatLog(L"Log it %d", *(char*)p);

#if defined(FAT_OS_WINDOWS)
	void* p1 = FatRealloc(p, 15);
	*(char*)p1 = 2;
#endif
}

void UnitTestApp::Shutdown()
{
#if defined(FAT_ENABLE_UNITTEST)
	theFakeFactory->Shutdown();
#endif

	Application::Shutdown();
}
