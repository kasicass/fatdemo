#pragma once

namespace Fat {

class ICommandData;
class IServerDevice;

class IServerCommand : public IServerObject
{
public:
	IServerCommand() {}
	virtual ~IServerCommand() {}

protected:
	friend class Packet;

	// Build according to a new data
	virtual void Build(const ICommandData* pData, const IServerDevice* pServerDevice) = 0;

	// Dispatch to specific API
	virtual void Dispatch(const ICommandData* pData, IServerDevice* pServerDevice) = 0;
};

}
