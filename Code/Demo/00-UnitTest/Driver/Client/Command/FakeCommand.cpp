#include "FatFramework/FatFramework.h"
#include "Demo/00-UnitTest/Driver/Client/Command/FakeCommand.h"

#if defined(FAT_ENABLE_UNITTEST)

//
// FakeCommandData
//

FakeCommandData::FakeCommandData() :
	value_(0)
{
}

FakeCommandData::~FakeCommandData()
{
}

void FakeCommandData::SetValue(UInt32 value)
{
	value_ = value;
}

UInt32 FakeCommandData::GetValue() const
{
	return value_;
}

void FakeCommandData::Clear()
{
	value_ = 0;
}

void FakeCommandData::Copy(const ICommandData* rhs)
{
	value_ = ((const FakeCommandData*)rhs)->value_;
}

//
// FakeCheckCommandData
//

FakeCheckCommandData::FakeCheckCommandData() :
	value_(0)
{
}

FakeCheckCommandData::~FakeCheckCommandData()
{
}

void FakeCheckCommandData::SetValue(UInt32 value)
{
	value_ = value;
}

UInt32 FakeCheckCommandData::GetValue() const
{
	return value_;
}

void FakeCheckCommandData::Clear()
{
	value_ = 0;
}

void FakeCheckCommandData::Copy(const ICommandData* rhs)
{
	value_ = ((const FakeCheckCommandData*)rhs)->value_;
}

#endif
