#pragma once

namespace Fat {

class ICommandData;
class IServerDevice;

class IServerCommand : public IServerObject
{
public:
	IServerCommand() {}
	virtual ~IServerCommand() {}

private:
	friend class Packet;

	// Build according to a new data
	virtual void Build(const ICommandData& data, const IServerDevice& device) = 0;

	// Dispatch to specific API
	virtual void Dispatch(const ICommandData& data, IServerDevice& device) = 0;
};

}
