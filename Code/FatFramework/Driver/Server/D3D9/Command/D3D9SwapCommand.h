#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"

#if FAT_OS_WINDOWS

#include "FatFramework/Driver/Server/Interface/IServerCommand.h"

namespace Fat {

class D3D9SwapCommand : public IServerCommand
{
protected:
	virtual void Build(const ICommandData* pData, const IServerDevice* pServerDevice) override;
	virtual void Dispatch(const ICommandData* pData, IServerDevice* pServerDevice) override;
};

}

#endif
