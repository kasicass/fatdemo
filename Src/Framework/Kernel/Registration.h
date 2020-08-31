#pragma once

namespace Fat {

FAT_ENUM_BEGIN(ERegisterPrimaryPriority)
	eReserved,		// Reserved for memory management tasks

	eHighest,
	eVeryHigh,
	eHigh,
	eAboveNormal,
	eNormal,
	eBelowNormal,
	eVeryLow,
	eLowest,

#if defined(FAT_USE_UNITTEST)
	eUnitTest,		// Unit tests execution
#endif
FAT_ENUM_END()

FAT_ENUM_BEGIN(ERegisterSecondaryPriority)
	eHighest,
	eVeryHigh,
	eHigh,
	eAboveNormal,
	eNormal,
	eBelowNormal,
	eVeryLow,
	eLowest,
FAT_ENUM_END()

FAT_ENUM_BEGIN(ERegisteredOperation)
	eInitialize,
	eDestory,
FAT_ENUM_END()

class IRegisteredObject : private INonCopyable
{
public:
	IRegisteredObject(ERegisterPrimaryPriority::EValue primaryPrio, ERegisterSecondaryPriority::EValue secondaryPrio);
	virtual ~IRegisteredObject();

private:
	static Bool IsLess(const IRegisteredObject* lhs, const IRegisteredObject* rhs);

private:
	friend class Registration;

	virtual void operator()(ERegisteredOperation::EValue op) = 0;

	ERegisterPrimaryPriority::EValue primaryPrio_;
	ERegisterSecondaryPriority::EValue secondaryPrio_;
};

class RegisteredFunction : public IRegisteredObject
{
public:
	typedef void (*TFuncType)(ERegisteredOperation::EValue op);

	RegisteredFunction(TFuncType pFunc, ERegisterPrimaryPriority::EValue primaryPrio, ERegisterSecondaryPriority::EValue secondaryPrio);

private:
	virtual void operator()(ERegisteredOperation::EValue op) override;

	TFuncType pFunc_;
};

class Registration
{
public:
	static void CallInitialize();
	static void CallDestroy();

private:
	friend class IRegisteredObject;

	static void RegisterObject(IRegisteredObject* pObject);
	static void UnregisterObject(IRegisteredObject* pObject);

	typedef std::list<IRegisteredObject*> RegisteredList;
	static RegisteredList* s_registeredObjects;
};

}

#define FAT_REGISTER_OBJECT(ClassName, ...) \
	static ClassName FAT_CONCAT(_fatRegisteredObject, __COUNTER__)(__VAR_ARGS__)

#define FAT_REGISTER_FUNCTION(_func, _primaryPrio, _secondaryPrio) \
	FAT_REGISTER_OBJECT(Fat::RegisteredFunction, _func, _primaryPrio, _secondaryPrio)

