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
	int* p = (int*)FatMalloc(sizeof(int)*10);
	MemorySet(p, 0, sizeof(int)*10);
	FatFree(p);

	FatLog(L"UnitTestApp::Run()");

	p = FatNew(int, 10);
	FatDelete(p);
}
