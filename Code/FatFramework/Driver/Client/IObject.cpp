#include "FatFramework/Driver/Client/IObject.h"
#include "FatFramework/Driver/Client/FactorySelector.h"

namespace Fat {

IObject::IObject(EFactoryObject::EValue value) :
	pServerObject_(GDriverFactorySelector->Instantiate(value))
{
}

IObject::~IObject()
{
}

IServerObject* IObject::GetServerObject() const
{
	return pServerObject_.Get();
}

}
