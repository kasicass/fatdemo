#pragma once

#include "FatFramework/Kernel/Common/SmartPtr.h"
#include "FatFramework/Kernel/Common/NonCopyable.h"
#include "FatFramework/Driver/Client/IObject.h"
#include "FatFramework/Driver/Client/CommandLocker.h"
#include "FatFramework/Driver/Server/Interface/IServerCommand.h"
#include <list>

namespace Fat {

//
// ICommandData - client command data object interface
//

class ICommandData : public IntrusiveCounter, private NonCopyable
{
public:
	ICommandData();
	virtual ~ICommandData();

	virtual void Clear() = 0;
	virtual void Copy(const ICommandData* rhs) = 0;

	bool IsAcquired() const;
	bool IsReadOnlyAcquired() const;
	bool IsReadWriteAcquired() const;

private:
	friend class CommandLockerBase;
	void AcquireReadOnlyAccess() const;
	void ReleaseReadOnlyAccess() const;
	void AcquireReadWriteAccess();
	void ReleaseReadWriteAccess();

	friend class Packet;
	bool IsRebuildNeeded() const;
	void Rebuilt() const;

private:
	//  0 - no lock
	// >0 - read-lock
	// -1 - write-lock
	mutable AtomicInt acquiredCounter_;
	mutable bool rebuildNeeded_;
};

typedef TSmartPtr<ICommandData, TSmartPtrIntrusivePolicy> ICommandDataPtr;

//
// ICommand - client command object interface
//

class ICommand : protected IObject
{
public:
	ICommand(ICommandData* pData, EFactoryObject::EValue value);
	virtual ~ICommand();

private:
	virtual ICommandData* AllocateData() = 0;

	friend class CommandLockerBase;
	IServerCommand* GetServerCommand() const;
	const ICommandData* GetReadOnlyData() const;

	// If discard is true, then the data is initialized with default values, otherwise it
	// maintains previous ones
	ICommandData* GrabReadWriteData(bool discard);

private:
	typedef std::list<ICommandDataPtr> CommandDataList;
	CommandDataList commandDataList_;
};

//
// TCommandWrapper - implements ICommand virtual and shared code
//
template <typename t_CommandData, EFactoryObject::EValue t_eValue>
class TCommandWrapper : public ICommand
{
public:
	typedef TReadOnlyLocker<t_CommandData>  ReadOnlyLocker;
	typedef TReadWriteLocker<t_CommandData> ReadWriteLocker;

	TCommandWrapper() :
		ICommand(AllocateData(), t_eValue)
	{
	}

	virtual ~TCommandWrapper()
	{
	}

private:
	virtual ICommandData* AllocateData() override
	{
		return FatNew(t_CommandData);
	}
};

}
