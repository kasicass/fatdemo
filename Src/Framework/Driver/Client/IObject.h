#pragma once

#include "Kernel/Common/NonCopyable.h"
#include "Driver/Client/FactoryType.h"
#include "Driver/Server/Interface/IServerObject.h"

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
