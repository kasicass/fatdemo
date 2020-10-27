#pragma once

#if FAT_ENABLE_UNITTEST

class FakeDevice : public Fat::IServerDevice
{
public:
	FakeDevice();
	virtual ~FakeDevice();

	void ForceResetNeeded();
};
extern FakeDevice* theFakeDevice;

#endif
