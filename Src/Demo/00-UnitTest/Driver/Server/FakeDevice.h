#pragma once

class FakeDevice : public Fat::IServerDevice
{
public:
	FakeDevice();
	virtual ~FakeDevice();

	void ForceResetNeeded();
};
extern FakeDevice* theFakeDevice;
