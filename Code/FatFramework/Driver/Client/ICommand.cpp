#include "FatFramework/Driver/Client/ICommand.h"

namespace Fat {

//
// ICommandData
//

ICommandData::ICommandData() :
	acquiredCounter_(0),
	rebuildNeeded_(true)
{
}

ICommandData::~ICommandData()
{
	FatAssert(IsAcquired() == false, L"Deleting an acquired command data");
}

Bool ICommandData::IsAcquired() const
{
	FatAssertNoText(acquiredCounter_ >= -1);
	return acquiredCounter_ != 0;
}

Bool ICommandData::IsReadOnlyAcquired() const
{
	FatAssertNoText(acquiredCounter_ >= -1);
	return acquiredCounter_ > 0;
}

Bool ICommandData::IsReadWriteAcquired() const
{
	FatAssertNoText(acquiredCounter_ >= -1);
	return acquiredCounter_ == -1;
}

void ICommandData::AcquireReadOnlyAccess() const
{
	FatAssertNoText(IsReadWriteAcquired() == false);
	acquiredCounter_++;
}

void ICommandData::ReleaseReadOnlyAccess() const
{
	FatAssertNoText(IsReadOnlyAcquired() == true);
	acquiredCounter_--;
}

void ICommandData::AcquireReadWriteAccess()
{
	FatAssertNoText(IsReadOnlyAcquired() == false && IsReadWriteAcquired() == false);
	acquiredCounter_--;
	rebuildNeeded_ = true;
}

void ICommandData::ReleaseReadWriteAccess()
{
	FatAssertNoText(IsReadWriteAcquired() == true);
	acquiredCounter_++;
}

Bool ICommandData::IsRebuildNeeded() const
{
	FatAssertNoText(IsReadOnlyAcquired());
	return rebuildNeeded_;
}

void ICommandData::Rebuilt() const
{
	FatAssertNoText(IsReadOnlyAcquired());
	FatAssertNoText(rebuildNeeded_ == true);
	rebuildNeeded_ = false;
}

//
// ICommand
//

ICommand::ICommand(ICommandData* pData, EFactoryObject::EValue value) :
	IObject(value)
{
	commandDataList_.push_front(pData);
}

ICommand::~ICommand()
{
	commandDataList_.clear();
}

IServerCommand* ICommand::GetServerCommand() const
{
	return (IServerCommand*)GetServerObject();
}

const ICommandData* ICommand::GetReadOnlyData() const
{
	FatAssertNoText(commandDataList_.empty() == false);

	// the front of the list data is always the most recent
	const ICommandData* pData = commandDataList_.front().Get();
	FatAssert(pData->IsReadWriteAcquired() == false, L"This data is already locked for write on the client side");

	return pData;
}

ICommandData* ICommand::GrabReadWriteData(Bool discard)
{
	FatAssertNoText(commandDataList_.empty() == false);

	// Stores the latest used data as it might be required to copy it to a newly allocated one
	ICommandData* pLastestData = commandDataList_.front().Get();

	// Iterate through all data in order to get one that isn't acquired for read purpose.
	// The first one would be great as it could avoid calling the copy
	CommandDataList::iterator it = commandDataList_.begin();
	for (; it != commandDataList_.end(); ++it)
	{
		ICommandData& data = **it;
		if (!data.IsReadOnlyAcquired())
		{
			// got it
			break;
		}
	}

	// Allocate a new data if none was available in the list

	ICommandData* pGrabbedData = NULL;
	if (it == commandDataList_.end())
	{
		pGrabbedData = AllocateData();
	}
	else
	{
		pGrabbedData = it->Get();
	}

	// Now update data list in order to maintain the latest data at the front

	if (it != commandDataList_.begin())
	{
		commandDataList_.push_front(pGrabbedData);
		if (it != commandDataList_.end())
		{
			commandDataList_.erase(it);
		}
	}

	// Now handles "discarding behaviour"
	// Copy the data if discard isn't required, otherwise clear it
	if (!discard)
	{
		// Skip copy if source and destination data are the same. It can be the case if the data found in
		// the search loop was the front one.
		if (pGrabbedData != pLastestData)
		{
			pGrabbedData->Copy(pLastestData);
		}
	}
	else
	{
		// Discard is accomplished by clearing the data
		pGrabbedData->Clear();
	}

	FatAssertNoText(pGrabbedData->IsAcquired() == false);
	return pGrabbedData;
}

}
