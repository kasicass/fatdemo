#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"

#if FAT_OS_WINDOWS

#include "FatFramework/Driver/Server/Interface/IServerState.h"
#include "FatFramework/Driver/Server/D3D9/D3D9Common.h"

namespace Fat {

class D3D9RenderTargetState : public IServerState
{
protected:
	virtual void Build(const ICommandData* pData, const IServerDevice* pServerDevice) override;
	virtual void Dispatch(const ICommandData* pData, IServerDevice* pServerDevice) override;

private:
	D3DCOLOR color_;
};

}

#endif
