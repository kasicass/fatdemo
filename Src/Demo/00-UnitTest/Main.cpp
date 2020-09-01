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
	printf("UnitTestApp::Run()\n");
	FatFree(p);

	p = FatNew(int, 10);
	FatDelete(p);
}
