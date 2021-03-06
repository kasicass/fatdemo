#pragma once

#include "FatFramework/Driver/Client/FactoryType.h"

namespace Fat {

class IServerObject;

// Server object factory. A minimal factory used to create platform server objects matching a client one
class IServerFactory
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual IServerObject* Instantiate(EFactoryObject::EValue value) = 0;

protected:
	void RegisterMe(EGraphicAPI::EValue api);
};

}
