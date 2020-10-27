#include "FatFramework/FatFramework.h"
#include "Demo/00-UnitTest/Driver/Server/FakeFactory.h"

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
	
#if FAT_ENABLE_UNITTEST
	GDriverFakeFactory->Init();
	TEST_REGISTER(UnitTestSelfTest);
	TEST_REGISTER(TestFloat3);
	TEST_REGISTER(TestFloat4);
	TEST_REGISTER(TestTime);
	TEST_REGISTER(TestFpsCounter);
	TEST_REGISTER(TestInterlocked);
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
	TEST_REGISTER(TestStateCacheBind);
	GUnitTestMgr->Run();
#else
	FatLog(L"FAT_ENABLE_UNITTEST is off");
#endif

	void* p = FatMalloc(10);
	*(char*)p = 1;

#if FAT_OS_WINDOWS
	void* p1 = FatRealloc(p, 15);
	*(char*)p1 = 2;
#endif

	class MyClass
	{
	public:
		MyClass(int v) { value_ = v; }

		void Foo() {}

	private:
		int value_;
	};
	MyClass* my = FatNew(MyClass, 2);
	my->Foo();
}

void UnitTestApp::Shutdown()
{
#if FAT_ENABLE_UNITTEST
	GDriverFakeFactory->Shutdown();
#endif

	Application::Shutdown();
}
