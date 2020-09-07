#pragma once

namespace Fat {

class IState;
class ICommand;
class ICommandData;
class IServerCommand;

// Implement commands, states, and command data access functions. This is the only friend class that can
// access to commands, states, and command data, so every locker object should inherit this class
class CommandLockerBase : private NonCopyable
{
protected:
	CommandLockerBase() {}
	~CommandLockerBase() {}

	const ICommandData& GetReadOnlyData(const ICommand& command) const;
	ICommandData& GrabReadWriteData(ICommand& command, Bool discard) const;

	const ICommandData& GetReadOnlyData(const IState& state) const;
	ICommandData& GrabReadWriteData(IState& state, Bool discard) const;

	void AcquireReadOnlyAccess(const ICommandData& data) const;
	void ReleaseReadOnlyAccess(const ICommandData& data) const;
	
	void AcquireReadWriteAccess(const ICommandData& data) const;
	void ReleaseReadWriteAccess(const ICommandData& data) const;

	IServerCommand& GetServerCommand(const ICommand& command) const;
	IServerCommand& GetServerCommand(const IState& state) const;
};

// Implement a read only locker object. It provides a const access on the locked data in order to
// ensure at compilation time that no "write" function is called. It is based on the RAII pattern
// in order to ensure locking-unlocking symmetry
template <typename T>
class TReadOnlyCommandLocker : protected CommandLockerBase
{
public:
	TReadOnlyCommandLocker(const ICommand& command) :
		pCommandData_(&GetReadOnlyData(command))
	{
		AcquireReadOnlyAccess(*pCommandData_);
	}

	TReadOnlyCommandLocker(const IState& state) :
		pCommandData_(&GetReadOnlyData(state))
	{
		AcquireReadOnlyAccess(*pCommandData_);
	}

	~TReadOnlyCommandLocker()
	{
		ReleaseReadOnlyAccess(*pCommandData_);
	}

	void operator=(const TReadOnlyCommandLocker& rhs)
	{
		if (this == &rhs)
			return;

		ReleaseReadOnlyAccess(*pCommandData_);
		pCommandData_ = rhs.pCommandData_;
		AcquireReadOnlyAccess(*pCommandData_);
	}

	const T& GetData() const
	{
		return (T&)(*pCommandData_);
	}

protected:
	TReadOnlyCommandLocker()
	{
		FatAssertUnreachableCode();
	}

	TReadOnlyCommandLocker(const ICommandData& commandData) :
		pCommandData_(&commandData)
	{
		AcquireReadOnlyAccess(*pCommandData_);
	}

	TReadOnlyCommandLocker(const TReadOnlyCommandLocker& rhs) :
		pCommandData_(rhs.pCommandData_)
	{
		AcquireReadOnlyAccess(*pCommandData_);
	}

private:
	TSmartPtr<const ICommandData, TSmartPtrIntrusivePolicy> pCommandData_;
};

// Implement a read-write locker object. It provides a non-const access on the locked data such that
// read and write calls can be done. It is based on the RAII pattern in order to ensure locking-unlocking
// symmetry
template <typename T>
class TReadWriteCommandLocker : public CommandLockerBase
{
public:
	TReadWriteCommandLocker(ICommand& command, Bool discard = true) :
		pCommandData_(&GrabReadWriteData(command, discard))
	{
		AcquireReadWriteAccess(pCommandData_);
	}

	TReadWriteCommandLocker(IState& state, Bool discard = true) :
		pCommandData_(&GrabReadWriteData(state, discard))
	{
		AcquireReadWriteAccess(*pCommandData_);
	}

	~TReadWriteCommandLocker()
	{
		ReleaseReadWriteAccess(*pCommandData_);
	}

	T& GetData() const
	{
		return (T&)(*pCommandData_);
	}

protected:
	TReadWriteCommandLocker(ICommandData& commandData) :
		pCommandData_(&commandData)
	{
		AcquireReadWriteAccess(*pCommandData_);
	}

	TReadWriteCommandLocker(const TReadWriteCommandLocker& rhs) :
		pCommandData_(&rhs.pCommandData_)
	{
		AcquireReadWriteAccess(*pCommandData_);
	}

private:
	TSmartPtr<ICommandData, TSmartPtrIntrusivePolicy> pCommandData_;
};

// Implement a read-only locker object and holds the related server command. This class is used in the driver
// everywhere a reference to a command data and a server command is needed, ie: the state cache, the command
// buffer...
class TReadOnlyCommandLockerHolder : public TReadOnlyCommandLocker<ICommandData>
{
protected:
	typedef TReadOnlyCommandLocker<ICommandData> _MyBase;

	TReadOnlyCommandLockerHolder(const ICommand& command) :
		_MyBase(command),
		pServerCommand_(&_MyBase::GetServerCommand(command))
	{
	}

	TReadOnlyCommandLockerHolder(const IState& state) :
		_MyBase(state),
		pServerCommand_(&_MyBase::GetServerCommand(state))
	{
	}

	TReadOnlyCommandLockerHolder(IServerCommand& serverCommand, const ICommandData& commandData) :
		_MyBase(commandData),
		pServerCommand_(&serverCommand)
	{
	}

	TReadOnlyCommandLockerHolder(const TReadOnlyCommandLockerHolder& rhs) :
		_MyBase(rhs),
		pServerCommand_(rhs.pServerCommand_)
	{
	}

	TReadOnlyCommandLockerHolder()
	{
		FatAssertUnreachableCode();
	}

	~TReadOnlyCommandLockerHolder()
	{
	}

	void operator=(const TReadOnlyCommandLockerHolder& rhs)
	{
		if (this == &rhs)
			return;

		_MyBase::operator=(rhs);
		pServerCommand_ = rhs.pServerCommand_;
	}

	IServerCommand& GetServerCommand() const
	{
		return *pServerCommand_;
	}

private:
	TSmartPtr<IServerCommand, TSmartPtrIntrusivePolicy> pServerCommand_;
};

}
