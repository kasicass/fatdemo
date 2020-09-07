#include "FatFramework.h"

namespace Fat {

const ICommandData* CommandLockerBase::GetReadOnlyData(const ICommand* pCommand) const
{
	return pCommand->GetReadOnlyData();
}

ICommandData* CommandLockerBase::GrabReadWriteData(ICommand* pCommand, Bool discard) const
{
	return pCommand->GrabReadWriteData(discard);
}

const ICommandData* CommandLockerBase::GetReadOnlyData(const IState* pState) const
{
	return pState->GetCommand()->GetReadOnlyData();
}

ICommandData* CommandLockerBase::GrabReadWriteData(IState* pState, Bool discard) const
{
	return pState->GetCommand()->GrabReadWriteData(discard);
}

void CommandLockerBase::AcquireReadOnlyAccess(const ICommandData* pData) const
{
	pData->AcquireReadOnlyAccess();
}

void CommandLockerBase::ReleaseReadOnlyAccess(const ICommandData* pData) const
{
	pData->ReleaseReadOnlyAccess();
}
	
void CommandLockerBase::AcquireReadWriteAccess(ICommandData* pData) const
{
	pData->AcquireReadWriteAccess();
}

void CommandLockerBase::ReleaseReadWriteAccess(ICommandData* pData) const
{
	pData->ReleaseReadWriteAccess();
}

IServerCommand* CommandLockerBase::GetServerCommand(const ICommand* pCommand) const
{
	return pCommand->GetServerCommand();
}

IServerCommand* CommandLockerBase::GetServerCommand(const IState* pState) const
{
	return pState->GetCommand()->GetServerCommand();
}

}
