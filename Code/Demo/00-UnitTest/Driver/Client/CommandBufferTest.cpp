#include "FatFramework.h"

#if defined(FAT_ENABLE_UNITTEST)

using namespace Fat;

TEST_DECLARE(TestCommandBufferModes)
{
	FactoryRAIISelector raiiSelector(EGraphicAPI::eUnitTest);
	Device device;

	FAT_ENUM_FOREACH(mode, EBufferingMode)
	{
		CommandBuffer commandBuffer(device, mode);

		// Change mode dynamically
		FAT_ENUM_FOREACH(newMode, EBufferingMode)
		{
			// Simulate many frames
			for (int i = 0; i < 10; ++i)
			{
				commandBuffer.Begin();
				commandBuffer.End();
			}

			// Change mode
			commandBuffer.SetBufferingMode(newMode);

			// Changing modes should fail inside a begin end
			commandBuffer.Begin();
			FatTestAssert(commandBuffer.SetBufferingMode(newMode));
			commandBuffer.End();
		}
	}
}

#endif
