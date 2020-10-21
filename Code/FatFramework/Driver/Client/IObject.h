#pragma once

#include "FatFramework/Kernel/Common/NonCopyable.h"
#include "FatFramework/Driver/Client/FactoryType.h"
#include "FatFramework/Driver/Server/Interface/IServerObject.h"

namespace Fat {

class IObject : private NonCopyable
{
public:
	IObject(EFactoryObject::EValue value);
	virtual ~IObject();

protected:
	IServerObject* GetServerObject() const;

private:
	IServerObjectPtr pServerObject_;
};

}
