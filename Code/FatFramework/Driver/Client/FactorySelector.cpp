#include "FatFramework/Driver/Client/FactorySelector.h"
#include "FatFramework/Kernel/Common/Log.h"
#include "FatFramework/Kernel/Common/Assertion.h"
#include "FatFramework/Driver/Server/Interface/IServerFactory.h"
#include <vector>

namespace Fat {

//
// FactorySelector
//

class FactorySelector : public IFactorySelector
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	virtual void SelectorFactory(EGraphicAPI::EValue api) override;
	virtual void UnselectFactory() override;

private:
	virtual IServerObject* Instantiate(EFactoryObject::EValue value) override;
	virtual void RegisterFactory(EGraphicAPI::EValue api, IServerFactory* factory) override;

private:
	typedef std::vector<IServerFactory*> ServerFactoryVector;
	ServerFactoryVector factories_;

	IServerFactory* pSelectedFactory_;
};

void FactorySelector::Init()
{
	FatLog(L"<Driver>: FactorySelector Init");
	pSelectedFactory_ = NULL;
	factories_.resize(EGraphicAPI::eValuesCount);
}

void FactorySelector::Shutdown()
{
	FatLog(L"<Driver>: FactorySelector Shutdown");
	factories_.clear();
	pSelectedFactory_ = NULL;
}

void FactorySelector::SelectorFactory(EGraphicAPI::EValue api)
{
	pSelectedFactory_ = factories_[api];
	FatAssert(pSelectedFactory_ != NULL, L"No factory registered for API %d", api);
}

void FactorySelector::UnselectFactory()
{
	pSelectedFactory_ = NULL;
}

IServerObject* FactorySelector::Instantiate(EFactoryObject::EValue value)
{
	FatAssert(pSelectedFactory_ != NULL, L"No factory selected");
	return pSelectedFactory_->Instantiate(value);
}

void FactorySelector::RegisterFactory(EGraphicAPI::EValue api, IServerFactory* factory)
{
	FatAssert(factories_[api] == NULL, L"A factory is already registered for API %d", api);
	factories_[api] = factory;
}

static FactorySelector myFactorySelector;
IFactorySelector* theFactorySelector = &myFactorySelector;

//
// FactoryRAIISelector
//

FactoryRAIISelector::FactoryRAIISelector(EGraphicAPI::EValue api)
{
	theFactorySelector->SelectorFactory(api);
}

FactoryRAIISelector::~FactoryRAIISelector()
{
	theFactorySelector->UnselectFactory();
}

}
