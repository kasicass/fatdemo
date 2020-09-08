#pragma once

#if defined(FAT_ENABLE_UNITTEST)

class IFakeFactory : public Fat::IServerFactory
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
};
extern IFakeFactory* theFakeFactory;

#endif
