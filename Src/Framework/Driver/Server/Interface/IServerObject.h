#pragma once

#include "Kernel/Common/SmartPtr.h"
#include "Kernel/Common/NonCopyable.h"

namespace Fat {

class IServerObject : public IntrusiveCounter, private NonCopyable
{
public:
	IServerObject() {}
	virtual ~IServerObject() {}
};

typedef TSmartPtr<IServerObject, TSmartPtrIntrusivePolicy> IServerObjectPtr;

}
