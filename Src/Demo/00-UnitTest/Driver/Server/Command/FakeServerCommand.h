#pragma once

#if defined(FAT_ENABLE_UNITTEST)

class FakeServerCommand : public Fat::IServerCommand
{
protected:
	virtual void Build(const Fat::ICommandData* pData, const Fat::IServerDevice* pServerDevice) override;
	virtual void Dispatch(const Fat::ICommandData* pData, Fat::IServerDevice* pServerDevice) override;

private:
	UInt32 builtValue_;
};

class FakeCheckServerCommand : public Fat::IServerCommand
{
public:
	static void SetValueToTest(UInt32 value);

protected:
	virtual void Build(const Fat::ICommandData* pData, const Fat::IServerDevice* pServerDevice) override;
	virtual void Dispatch(const Fat::ICommandData* pData, Fat::IServerDevice* pServerDevice) override;

private:
	UInt32 builtValue_;

	static UInt32 s_valueToTest;

};

#endif

