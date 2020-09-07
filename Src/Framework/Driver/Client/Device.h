#pragma once

namespace Fat {

class Device : protected IObject
{
public:
	Device();
	virtual ~Device();

	// Execute device reset. This function is executed synchronously on the server side, so the device must
	// have been properly stopped.
	Bool Reset();

	Bool IsResetNeeded() const;

private:
	friend class Packet;
	IServerDevice* GetServerDevice() const;
};

}
