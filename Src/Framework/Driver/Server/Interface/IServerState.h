#pragma once

#include "Driver/Server/Interface/IServerCommand.h"

namespace Fat {

class IServerState : public IServerCommand
{
public:
	IServerState() {}
	virtual ~IServerState() {}
};

}
