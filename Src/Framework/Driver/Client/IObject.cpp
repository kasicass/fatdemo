#include "FatFramework.h"

namespace Fat {

IObject::IObject(EFactoryObject::EValue value) :
	pServerObject_(theFactorySelector->Instantiate(value))
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