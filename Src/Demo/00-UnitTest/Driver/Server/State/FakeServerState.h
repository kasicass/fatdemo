#pragma once

#if defined(FAT_ENABLE_UNITTEST)

class FakeDefaultServerState : public Fat::IServerState
{
protected:
	// Build according to a new data
	virtual void Build(const Fat::ICommandData* pData, const Fat::IServerDevice* pServerDevice) override;

	// Dispatch to specific API
	virtual void Dispatch(const Fat::ICommandData* pData, Fat::IServerDevice* pServerDevice) override;
};

class FakeRenderTargetServerState : public Fat::IServerState
{
protected:
	// Build according to a new data
	virtual void Build(const Fat::ICommandData* pData, const Fat::IServerDevice* pServerDevice) override;

	// Dispatch to specific API
	virtual void Dispatch(const Fat::ICommandData* pData, Fat::IServerDevice* pServerDevice) override;

private:
	UInt32 builtValue_;
};

#endif
