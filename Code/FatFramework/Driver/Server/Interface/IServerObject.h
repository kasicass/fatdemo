#pragma once

#include "FatFramework/Kernel/Common/SmartPtr.h"
#include "FatFramework/Kernel/Common/NonCopyable.h"

namespace Fat {

class IServerObject : public IntrusiveCounter, private NonCopyable
{
public:
	IServerObject() {}
	virtual ~IServerObject() {}
};

typedef TSmartPtr<IServerObject, TSmartPtrIntrusivePolicy> IServerObjectPtr;

}
