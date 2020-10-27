#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"

#if defined(FAT_OS_WINDOWS)

#include "FatFramework/Driver/Server/Interface/IServerFactory.h"

namespace Fat {

class D3D9Factory : public IServerFactory
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	virtual IServerObject* Instantiate(EFactoryObject::EValue value) override;
};
extern IServerFactory* GDriverD3D9Factory;

}

#endif
