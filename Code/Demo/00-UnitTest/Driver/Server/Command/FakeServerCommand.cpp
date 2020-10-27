#include "FatFramework/FatFramework.h"
#include "Demo/00-UnitTest/Driver/Server/Command/FakeServerCommand.h"
#include "Demo/00-UnitTest/Driver/Client/Command/FakeCommand.h"

#if FAT_ENABLE_UNITTEST

//
// FakeServerCommand
//

void FakeServerCommand::Build(const Fat::ICommandData* pDataIn, const Fat::IServerDevice* pServerDevice)
{
	const FakeCommandData* pData = (const FakeCommandData*)pDataIn;
	builtValue_ = pData->GetValue();
}

void FakeServerCommand::Dispatch(const Fat::ICommandData* pDataIn, Fat::IServerDevice* pServerDevice)
{
	const FakeCommandData* pData = (const FakeCommandData*)pDataIn;
	FatTest(builtValue_ == pData->GetValue());
	FakeCheckServerCommand::SetValueToTest(builtValue_);
}

//
// FakeCheckServerCommand
//

UInt32 FakeCheckServerCommand::s_valueToTest = 0;

void FakeCheckServerCommand::SetValueToTest(UInt32 value)
{
	s_valueToTest = value;
}

void FakeCheckServerCommand::Build(const Fat::ICommandData* pDataIn, const Fat::IServerDevice* pServerDevice)
{
	const FakeCheckCommandData* pData = (const FakeCheckCommandData*)pDataIn;
	builtValue_ = pData->GetValue();
}

void FakeCheckServerCommand::Dispatch(const Fat::ICommandData* pDataIn, Fat::IServerDevice* pServerDevice)
{
	const FakeCheckCommandData* pData = (const FakeCheckCommandData*)pDataIn;
	FatTest(builtValue_ == pData->GetValue());
	FatTest(builtValue_ == s_valueToTest);
}

#endif
