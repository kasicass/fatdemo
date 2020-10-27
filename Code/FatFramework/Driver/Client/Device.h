#pragma once

#include "FatFramework/Driver/Client/IObject.h"

namespace Fat {

class IServerDevice;
class Device : protected IObject
{
public:
	Device();
	virtual ~Device();

	// Execute device reset. This function is executed synchronously on the server side, so the device must
	// have been properly stopped.
	bool Reset();

	bool IsResetNeeded() const;

private:
	friend class Packet;
	IServerDevice* GetServerDevice() const;
};

}
