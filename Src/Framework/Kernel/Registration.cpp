#include "FatFramework.h"

namespace Fat {

//
// IRegisteredObject
//

IRegisteredObject::IRegisteredObject(ERegisterPrimaryPriority::EValue primaryPrio, ERegisterSecondaryPriority::EValue secondaryPrio) :
	primaryPrio_(primaryPrio),
	secondaryPrio_(secondaryPrio)
{
	Registration::RegisterObject(this);
}

IRegisteredObject::~IRegisteredObject()
{
	Registration::UnregisterObject(this);
}

Bool IRegisteredObject::IsLess(const IRegisteredObject* lhs, const IRegisteredObject* rhs)
{
	return (lhs->primaryPrio_ < rhs->primaryPrio_) ||
		((lhs->primaryPrio_ == rhs->primaryPrio_) && (lhs->secondaryPrio_ < rhs->secondaryPrio_));
}

//
// RegisteredFunction
//

RegisteredFunction::RegisteredFunction(TFuncType pFunc, ERegisterPrimaryPriority::EValue primaryPrio, ERegisterSecondaryPriority::EValue secondaryPrio) :
	IRegisteredObject(primaryPrio, secondaryPrio),
	pFunc_(pFunc)
{
	// TODO
}

void RegisteredFunction::operator()(ERegisteredOperation::EValue op)
{
	(*pFunc_)(op);
}

}
