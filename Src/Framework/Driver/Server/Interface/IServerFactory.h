#pragma once

namespace Fat {

// Server object factory. A minimal factory used to create platform server objects matching a client one
class IServerFactory
{
public:
	IServerFactory(EGraphicAPI::EValue api);
	virtual ~IServerFactory();

	virtual IServerObject* Instantiate(EFactoryObject::EValue value) = 0;
};

}
