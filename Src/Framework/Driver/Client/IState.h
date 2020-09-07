#pragma once

namespace Fat {

class IStateData : public ICommandData
{
public:
	IStateData();
	virtual ~IStateData();
};

// client state object interface. Implement state cache binding functionalities. A internal
// command object allows to queue the server state object to the command buffer
class IState : public NonCopyable
{
public:
	IState(EStateType::EValue stateType, ICommand* pCommand);
	virtual ~IState();

	EStateType::EValue GetStateType() const;

private:
	friend class CommandLockerBase;

	ICommand* GetCommand();
	const ICommand* GetCommand() const;

private:
	EStateType::EValue stateType_;
	ICommand* pCommand_;
};

template <typename t_StateData, EStateType::EValue t_eStateType, EFactoryObject::EValue t_eValue>
class TStateWrapper : public IState
{
public:
	typedef TReadOnlyLocker<t_StateData>  ReadOnlyLocker;
	typedef TReadWriteLocker<t_StateData> ReadWriteLocker;

	TStateWrapper() :
		IState(t_eStateType, &command_),
		command_()
	{
	}

private:
	TCommandWrapper<t_StateData, t_eValue> command_;
};

}
