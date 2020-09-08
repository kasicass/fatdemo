#pragma once

#if defined(FAT_ENABLE_UNITTEST)

class FakeCommandData : public Fat::ICommandData
{
public:
	FakeCommandData();
	virtual ~FakeCommandData();

	void SetValue(UInt32 value);
	UInt32 GetValue() const;

	virtual void Clear() override;
	virtual void Copy(const ICommandData* rhs);

private:
	UInt32 value_;
};

typedef Fat::TCommandWrapper<FakeCommandData, Fat::EFactoryObject::eUnitTestCommand> FakeCommand;


class FakeCheckCommandData : public Fat::ICommandData
{
public:
	FakeCheckCommandData();
	virtual ~FakeCheckCommandData();

	void SetValue(UInt32 value);
	UInt32 GetValue() const;

	virtual void Clear() override;
	virtual void Copy(const ICommandData* rhs);

private:
	UInt32 value_;
};

typedef Fat::TCommandWrapper<FakeCheckCommandData, Fat::EFactoryObject::eUnitTestCheckCommand> FakeCheckCommand;

#endif
