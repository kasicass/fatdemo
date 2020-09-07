#pragma once

class FakeDevice : public Fat::IServerDevice
{
public:
	FakeDevice();
	virtual ~FakeDevice();

	void ForceResetNeeded();
};
