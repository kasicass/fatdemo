#include "FatFramework.h"

namespace Fat {

#if 0
const ICommandData& CommandLocker::GetReadOnlyData(const ICommand& command) const
{
	return command.
}

ICommandData& GrabReadWriteData(ICommand& command, Bool discard) const;

const ICommandData& GetReadOnlyData(const IState& state) const;
ICommandData& GrabReadWriteData(IState& state, Bool discard) const;

void AcquireReadOnlyAccess(const ICommandData& data) const;
void ReleaseReadOnlyAccess(const ICommandData& data) const;

void AcquireReadWriteAccess(const ICommandData& data) const;
void ReleaseReadWriteAccess(const ICommandData& data) const;

IServerCommand& GetServerCommand(const ICommand& command) const;
IServerCommand& GetServerCommand(const IState& state) const;
#endif

}
