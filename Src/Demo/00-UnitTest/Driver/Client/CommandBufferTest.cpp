#include "FatFramework.h"

#if defined(FAT_ENABLE_UNITTEST)

using namespace Fat;

TEST_DECLARE(TestCommandBufferModes)
{
	FactoryRAIISelector raiiSelector(EGraphicAPI::eUnitTest);
	Device device;

	for (EBufferingMode::EValue mode = EBufferingMode::eNoBuffering; mode < EBufferingMode::eValuesCount; mode = EBufferingMode::EValue(mode+1))
	{
		CommandBuffer commandBuffer(device, mode);

		// Change mode dynamically
		for (EBufferingMode::EValue newMode = EBufferingMode::eNoBuffering; newMode < EBufferingMode::eValuesCount; newMode = EBufferingMode::EValue(newMode+1))
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
