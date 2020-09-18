#include "FatFramework.h"

#if defined(FAT_OS_WINDOWS)

namespace Fat {

D3D9StateShadow::D3D9StateShadow()
{
	MemoryZero(serverStates_);
	MemoryZero(serverStateData_);
}

D3D9StateShadow::~D3D9StateShadow()
{
}

void D3D9StateShadow::SetState(EStateType::EValue stateType, const IServerObject* pState, const ICommandData* pData)
{
	serverStates_[stateType]    = pState;
	serverStateData_[stateType] = pData;
}

const IServerObject* D3D9StateShadow::GetState(EStateType::EValue stateType) const
{
	return serverStates_[stateType];
}

const ICommandData* D3D9StateShadow::GetStateData(EStateType::EValue stateType) const
{
	return serverStateData_[stateType];
}

void D3D9StateShadow::Release()
{
	FAT_ENUM_FOREACH(stateType, EStateType)
	{
		serverStates_[stateType] = NULL;
		serverStateData_[stateType] = NULL;
	}
}

}

#endif
