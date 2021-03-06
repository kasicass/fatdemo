#pragma once

#if FAT_ENABLE_UNITTEST

class FakeFactory : public Fat::IServerFactory
{
public:
	void Init() override;
	void Shutdown() override;

	virtual Fat::IServerObject* Instantiate(Fat::EFactoryObject::EValue value) override;
};
extern Fat::IServerFactory* GDriverFakeFactory;

#endif
