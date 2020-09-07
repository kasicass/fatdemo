#include "FatFramework.h"
#include "FakeFactory.h"
#include "FakeDevice.h"

using namespace Fat;

class FakeFactory : public IFakeFactory
{
public:
	void Init() override;
	void Shutdown() override;

	virtual IServerObject* Instantiate(EFactoryObject::EValue value) override;
};

static FakeFactory myFakeFactory;
IFakeFactory* theFakeFactory = &myFakeFactory;

void FakeFactory::Init()
{
	RegisterMe(EGraphicAPI::eUnitTest);
}

void FakeFactory::Shutdown()
{
}

IServerObject* FakeFactory::Instantiate(EFactoryObject::EValue value)
{
	switch (value)
	{
	case EFactoryObject::eDevice:
		return FatNew(FakeDevice);

	default:
		FatAssertUnreachableCode();
		return NULL;
	}
}
