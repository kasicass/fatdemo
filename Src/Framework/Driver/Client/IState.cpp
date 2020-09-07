#include "FatFramework.h"

namespace Fat {

//
// IState
//

IState::IState(EStateType::EValue stateType, ICommand* pCommand) :
	stateType_(stateType),
	pCommand_(pCommand)
{
}

IState::~IState()
{
}

EStateType::EValue IState::GetStateType() const
{
	return stateType_;
}

ICommand* IState::GetCommand()
{
	return pCommand_;
}

const ICommand* IState::GetCommand() const
{
	return pCommand_;
}

}
