#include "FatFramework/FatFramework.h"
#include "Demo/00-UnitTest/Driver/Server/State/FakeServerState.h"
#include "Demo/00-UnitTest/Driver/Server/Command/FakeServerCommand.h"

#if FAT_ENABLE_UNITTEST

using namespace Fat;

//
// FakeDefaultServerState
//

void FakeDefaultServerState::Build(const Fat::ICommandData* pData, const Fat::IServerDevice* pServerDevice)
{
}

void FakeDefaultServerState::Dispatch(const Fat::ICommandData* pData, Fat::IServerDevice* pServerDevice)
{
}

//
// FakeRenderTargetServerState
//

void FakeRenderTargetServerState::Build(const Fat::ICommandData* pDataIn, const Fat::IServerDevice* pServerDevice)
{
	const RenderTargetStateData* pData = (const RenderTargetStateData*)pDataIn;
	builtValue_ = pData->GetWidth();
}

void FakeRenderTargetServerState::Dispatch(const Fat::ICommandData* pDataIn, Fat::IServerDevice* pServerDevice)
{
	const RenderTargetStateData* pData = (const RenderTargetStateData*)pDataIn;
	FatTest(builtValue_ == pData->GetWidth());
	FakeCheckServerCommand::SetValueToTest(builtValue_);
}

#endif
