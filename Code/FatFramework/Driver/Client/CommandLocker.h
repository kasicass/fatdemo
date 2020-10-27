#pragma once

#include "FatFramework/Kernel/Common/NonCopyable.h"
#include "FatFramework/Kernel/Common/SmartPtr.h"

namespace Fat {

class IState;
class ICommand;
class IServerCommand;
class ICommandData;

typedef TSmartPtr<IServerCommand, TSmartPtrIntrusivePolicy> IServerCommandPtr;
typedef TSmartPtr<const ICommandData, TSmartPtrIntrusivePolicy> ICommandDataConstPtr;
typedef TSmartPtr<ICommandData, TSmartPtrIntrusivePolicy> ICommandDataPtr;

// Implement commands, states, and command data access functions. This is the only friend class that can
// access to commands, states, and command data, so every locker object should inherit this class
class CommandLockerBase : private NonCopyable
{
protected:
	const ICommandData* GetReadOnlyData(const ICommand* pCommand) const;
	ICommandData* GrabReadWriteData(ICommand* pCommand, bool discard) const;

	const ICommandData* GetReadOnlyData(const IState* pState) const;
	ICommandData* GrabReadWriteData(IState* pState, bool discard) const;

	void AcquireReadOnlyAccess(const ICommandData* pData) const;
	void ReleaseReadOnlyAccess(const ICommandData* pData) const;
	
	void AcquireReadWriteAccess(ICommandData* pData) const;
	void ReleaseReadWriteAccess(ICommandData* pData) const;

	IServerCommand* GetServerCommand(const ICommand* pCommand) const;
	IServerCommand* GetServerCommand(const IState* pState) const;
};

// Implement a read only locker object. It provides a const access on the locked data in order to
// ensure at compilation time that no "write" function is called. It is based on the RAII pattern
// in order to ensure locking-unlocking symmetry
template <typename T>
class TReadOnlyLocker : protected CommandLockerBase
{
public:
	TReadOnlyLocker(const ICommand* pCommand) :
		pCommandData_(GetReadOnlyData(pCommand))
	{
		AcquireReadOnlyAccess(pCommandData_.Get());
	}

	TReadOnlyLocker(const IState* pState) :
		pCommandData_(GetReadOnlyData(pState))
	{
		AcquireReadOnlyAccess(pCommandData_.Get());
	}

	~TReadOnlyLocker()
	{
		ReleaseReadOnlyAccess(pCommandData_.Get());
	}

	void operator=(const TReadOnlyLocker& rhs)
	{
		if (this == &rhs)
			return;

		ReleaseReadOnlyAccess(pCommandData_.Get());
		pCommandData_ = rhs.pCommandData_;
		AcquireReadOnlyAccess(pCommandData_.Get());
	}

	const T* GetData() const
	{
		return (T*)(pCommandData_.Get());
	}

protected:
	TReadOnlyLocker()
	{
		FatAssertUnreachableCode();
	}

	TReadOnlyLocker(const ICommandData* pData) :
		pCommandData_(pData)
	{
		AcquireReadOnlyAccess(pCommandData_.Get());
	}

	TReadOnlyLocker(const TReadOnlyLocker& rhs) :
		pCommandData_(rhs.pCommandData_)
	{
		AcquireReadOnlyAccess(pCommandData_.Get());
	}

private:
	ICommandDataConstPtr pCommandData_;
};

// Implement a read-write locker object. It provides a non-const access on the locked data such that
// read and write calls can be done. It is based on the RAII pattern in order to ensure locking-unlocking
// symmetry
template <typename T>
class TReadWriteLocker : public CommandLockerBase
{
public:
	TReadWriteLocker(ICommand* pCommand, bool discard = true) :
		pCommandData_(GrabReadWriteData(pCommand, discard))
	{
		AcquireReadWriteAccess(pCommandData_.Get());
	}

	TReadWriteLocker(IState* pState, bool discard = true) :
		pCommandData_(GrabReadWriteData(pState, discard))
	{
		AcquireReadWriteAccess(pCommandData_.Get());
	}

	~TReadWriteLocker()
	{
		ReleaseReadWriteAccess(pCommandData_.Get());
	}

	T* GetData() const
	{
		return (T*)(pCommandData_.Get());
	}

protected:
	TReadWriteLocker(ICommandData* pData) :
		pCommandData_(pData)
	{
		AcquireReadWriteAccess(pCommandData_.Get());
	}

	TReadWriteLocker(const TReadWriteLocker& rhs) :
		pCommandData_(rhs.pCommandData_)
	{
		AcquireReadWriteAccess(pCommandData_.Get());
	}

private:
	ICommandDataPtr pCommandData_;
};

// Implement a read-only locker object and holds the related server command. This class is used in the driver
// everywhere a reference to a command data and a server command is needed, ie: the state cache, the command
// buffer...
class ReadOnlyLockerHolder : public TReadOnlyLocker<ICommandData>
{
protected:
	typedef TReadOnlyLocker<ICommandData> _MyBase;

	ReadOnlyLockerHolder(const ICommand* pCommand) :
		_MyBase(pCommand),
		pServerCommand_(_MyBase::GetServerCommand(pCommand))
	{
	}

	ReadOnlyLockerHolder(const IState* pState) :
		_MyBase(pState),
		pServerCommand_(_MyBase::GetServerCommand(pState))
	{
	}

	ReadOnlyLockerHolder(IServerCommand* pServerCommand, const ICommandData* pCommandData) :
		_MyBase(pCommandData),
		pServerCommand_(pServerCommand)
	{
	}

	ReadOnlyLockerHolder(const ReadOnlyLockerHolder& rhs) :
		_MyBase(rhs),
		pServerCommand_(rhs.pServerCommand_)
	{
	}

	ReadOnlyLockerHolder()
	{
		FatAssertUnreachableCode();
	}

	~ReadOnlyLockerHolder()
	{
	}

	void operator=(const ReadOnlyLockerHolder& rhs)
	{
		if (this == &rhs)
			return;

		_MyBase::operator=(rhs);
		pServerCommand_ = rhs.pServerCommand_;
	}

	IServerCommand* GetServerCommand() const
	{
		return pServerCommand_.Get();
	}

private:
	IServerCommandPtr pServerCommand_;
};

}
