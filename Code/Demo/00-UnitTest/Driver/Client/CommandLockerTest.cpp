#include "FatFramework/FatFramework.h"

#if FAT_ENABLE_UNITTEST

using namespace Fat;

TEST_DECLARE(TestCommandLocker)
{
	class TestCommandData : public ICommandData
	{
	public:
		virtual void Clear() override {}
		virtual void Copy(const ICommandData* rhs) override {}
	} data;

	class TestLocker : protected CommandLockerBase
	{
	public:
		TestLocker(TestCommandData* pData)
		{
			// Read-only lock
			FatTestAssert(CommandLockerBase::ReleaseReadOnlyAccess(pData));
			CommandLockerBase::AcquireReadOnlyAccess(pData);
			CommandLockerBase::ReleaseReadOnlyAccess(pData);
			FatTestAssert(CommandLockerBase::ReleaseReadOnlyAccess(pData));

			// Read-write lock
			FatTestAssert(CommandLockerBase::ReleaseReadWriteAccess(pData));
			CommandLockerBase::AcquireReadWriteAccess(pData);
			FatTestAssert(CommandLockerBase::AcquireReadWriteAccess(pData));
			CommandLockerBase::ReleaseReadWriteAccess(pData);
			FatTestAssert(CommandLockerBase::ReleaseReadWriteAccess(pData));

			// Multiple read-write lock
			CommandLockerBase::AcquireReadOnlyAccess(pData);
			FatTestAssert(CommandLockerBase::AcquireReadWriteAccess(pData));
			CommandLockerBase::AcquireReadOnlyAccess(pData);
			FatTestAssert(CommandLockerBase::AcquireReadWriteAccess(pData));
			CommandLockerBase::ReleaseReadOnlyAccess(pData);
			CommandLockerBase::ReleaseReadOnlyAccess(pData);
			FatTestAssert(CommandLockerBase::ReleaseReadWriteAccess(pData));
			CommandLockerBase::AcquireReadWriteAccess(pData);
			CommandLockerBase::ReleaseReadWriteAccess(pData);
		}

	} locker(&data);
}

#endif
