#pragma once

#if defined(FAT_OS_WINDOWS)

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
