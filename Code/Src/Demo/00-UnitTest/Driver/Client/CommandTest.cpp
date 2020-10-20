#include "FatFramework.h"
#include "Driver/Client/Command/FakeCommand.h"

#if defined(FAT_ENABLE_UNITTEST)

using namespace Fat;

TEST_DECLARE(TestCommandLocking)
{
	FactoryRAIISelector raiiSelector(EGraphicAPI::eUnitTest);
	FakeCommand command;

	// Simple read-only lock
	{
		FakeCommand::ReadOnlyLocker locker(&command);
	}

	// Simple read-write lock
	{
		FakeCommand::ReadWriteLocker locker(&command);
	}

	// Multiple read-only locks
	{
		FakeCommand::ReadOnlyLocker locker1(&command);
		FakeCommand::ReadOnlyLocker locker2(&command);
		FakeCommand::ReadOnlyLocker locker3(&command);
		FakeCommand::ReadOnlyLocker locker4(&command);
	}

	// Mixed read-only and read-write locks
	{
		FakeCommand::ReadOnlyLocker locker1(&command);
		FakeCommand::ReadWriteLocker locker2(&command);
		FatTestAssert(FakeCommand::ReadOnlyLocker locker3(&command)); // trigger assertion in ICommand::GetReadOnlyData()
	}
}

TEST_DECLARE(TestCommandDataIntegrity)
{
	FactoryRAIISelector raiiSelector(EGraphicAPI::eUnitTest);
	FakeCommand command;

	// Simple read
	{
		FakeCommand::ReadOnlyLocker locker(&command);
		const FakeCommandData* pData = locker.GetData();
		FatTest(pData->GetValue() == 0);
	}

	// Simple write then read
	{
		// Modify the data
		{
			FakeCommand::ReadWriteLocker locker(&command);
			FakeCommandData* pData = locker.GetData();

			FatTest(pData->GetValue() == 0);
			pData->SetValue(12);
			FatTest(pData->GetValue() == 12);
		}

		// Ensure the data is modified when accessed with reads
		{
			FakeCommand::ReadOnlyLocker locker(&command);
			const FakeCommandData* pData = locker.GetData();
			FatTest(pData->GetValue() == 12);
		}
	}

	// If the data is modified again, then its value is reseted to default as "Discard" is the default
	// behaviour
	{
		FakeCommand::ReadWriteLocker locker(&command);
		FakeCommandData* pData = locker.GetData();

		FatTest(pData->GetValue() == 0);
		pData->SetValue(12);
		FatTest(pData->GetValue() == 12);
	}

	// If the data is modified again, then its value is not reseted if "Discard" flag is false
	{
		FakeCommand::ReadWriteLocker locker(&command, false);
		FakeCommandData* pData = locker.GetData();

		FatTest(pData->GetValue() == 12);
		pData->SetValue(21);
		FatTest(pData->GetValue() == 21);
	}

	// Even if data is acquired somewhere in the driver for read-only
	{
		FakeCommand::ReadOnlyLocker readLocker(&command);
		FakeCommand::ReadWriteLocker locker(&command, false);
		FakeCommandData* pData = locker.GetData();

		FatTest(pData->GetValue() == 21);
		pData->SetValue(12);
		FatTest(pData->GetValue() == 12);
	}
}

#endif
