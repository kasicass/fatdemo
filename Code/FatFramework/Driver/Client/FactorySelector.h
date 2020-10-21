#pragma once

#include "FatFramework/Driver/Client/FactoryType.h"

namespace Fat {

class IServerObject;
class IServerFactory;

// This minimal factory mechanism allow to instantiate a server object based on object type (EFactoryObject)
// and the selected graphic API (EGraphicAPI)
class IFactorySelector
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void SelectorFactory(EGraphicAPI::EValue api) = 0;
	virtual void UnselectFactory() = 0;

private:
	// IObject will call Instantiate()
	// Instantiate a server object form a client one (using client name as the identifier)
	friend class IObject;
	virtual IServerObject* Instantiate(EFactoryObject::EValue value) = 0;

	// Register a factory object for api
	friend class IServerFactory;
	virtual void RegisterFactory(EGraphicAPI::EValue api, IServerFactory* factory) = 0;
};
extern IFactorySelector* theFactorySelector;

class FactoryRAIISelector
{
public:
	FactoryRAIISelector(EGraphicAPI::EValue api);
	~FactoryRAIISelector();
};

}
