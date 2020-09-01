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
	FatAssert(pFunc_ != NULL, L"Registered function cannot be NULL");
}

void RegisteredFunction::operator()(ERegisteredOperation::EValue op)
{
	(*pFunc_)(op);
}

//
// Registration
//

Registration::RegisteredList* Registration::s_registeredObjects = NULL;

void Registration::RegisterObject(IRegisteredObject* pObject)
{
	if (s_registeredObjects == NULL)
		s_registeredObjects = FatNew(RegisteredList);

	s_registeredObjects->push_front(pObject);
}

void Registration::UnregisterObject(IRegisteredObject* pObject)
{
	FatAssertNoText(s_registeredObjects != NULL);

	s_registeredObjects->remove(pObject);

	if (s_registeredObjects->empty())
	{
		FatDelete(s_registeredObjects);
	}
}

void Registration::CallInitialize()
{
	if (s_registeredObjects == NULL)
		return;

	s_registeredObjects->sort(IRegisteredObject::IsLess);

	for (auto it = s_registeredObjects->begin(); it != s_registeredObjects->end(); ++it)
	{
		(*(*it))(ERegisteredOperation::eInitialize);
	}
}

void Registration::CallDestroy()
{
	if (s_registeredObjects == NULL)
		return;

	for (auto it = s_registeredObjects->rbegin(); it != s_registeredObjects->rend(); ++it)
	{
		(*(*it))(ERegisteredOperation::eDestory);
	}
}

}
