#pragma once

#if defined(FAT_OS_WINDOWS)

namespace Fat {

// D3D9 Server state shadow. Stores the server states that are bound when a command occurs. This is not the
// real right solution as it should rather be implemented in the server interface side.
class D3D9StateShadow final
{
public:
	D3D9StateShadow();
	~D3D9StateShadow();

	// Set a state and it associated data in the shadow
	void SetState(EStateType::EValue stateType, const IServerObject* pState, const ICommandData* pData);

	const IServerObject* GetState(EStateType::EValue stateType) const;
	const ICommandData* GetStateData(EStateType::EValue stateType) const;

	void Release();

private:
	const IServerObject* serverStates_[EStateType::eValuesCount];
	const ICommandData* serverStateData_[EStateType::eValuesCount];
};

}

#endif
