#include "FatFramework.h"
#include "Driver/Client/Command/FakeCommand.h"

#if defined(FAT_ENABLE_UNITTEST)

using namespace Fat;

TEST_DECLARE(TestStateCacheBind)
{
	FactoryRAIISelector raiiSelector(EGraphicAPI::eUnitTest);

	Device device;
	Context context(device);
	RenderTargetState rtState;
	FakeCheckCommand checkCommand;

	FAT_ENUM_FOREACH(mode, EBufferingMode)
	{
		context.SetBufferingMode(mode);

		for (UInt32 i = 0; i < 10; ++i)
		{
			// Enter Begin
			context.Begin();

			// Bind it for the first time, it should replace existing one in the cache
			FatTest(context.Bind(rtState));

			// Bind it agian without any lock, it doesn't update the cache
			FatTest(context.Bind(rtState) == false);

			// Bind the default state back
			FatTest(context.Reset(EStateType::eRenderTarget));

			// Re-bind it, it should replace default one in the cache
			FatTest(context.Bind(rtState));

			// Lock for read only
			{
				RenderTargetState::ReadOnlyLocker locker(&rtState);
			}

			// Bind it again, it doesn't update the cache either
			FatTest(context.Bind(rtState) == false);

			// Lock for read write now
			{
				RenderTargetState::ReadWriteLocker locker(&rtState);
			}

			// Bind it again, it updates the cache as the state was modified
			FatTest(context.Bind(rtState));

			// Bind the default state back
			FatTest(context.Reset(EStateType::eRenderTarget));

			// Lock for read write while the state isn't in the cache
			{
				RenderTargetState::ReadWriteLocker locker(&rtState);
			}

			// Bind it again, it updates the cache as the state was modified
			FatTest(context.Bind(rtState));

			// Bind the default state back
			FatTest(context.Reset(EStateType::eRenderTarget));

			context.End();
		}
	}
}

#endif
