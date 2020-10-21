#pragma once

#include "Kernel/PlatformConfig.h"
#include "Kernel/Common/Macros.h"

namespace Fat {

// Driver factory enum
FAT_ENUM_BEGIN(EGraphicAPI)
	eD3D9,
	eD3D11,
	eOpenGL,
#if defined(FAT_ENABLE_UNITTEST)
	eUnitTest,
#endif
FAT_ENUM_END()

// Driver factory object enum
FAT_ENUM_BEGIN(EFactoryObject)
	eDevice,

	// States
	eRenderTargetState,
	eVertexBufferState,
	eTopologyState,
	eMaterialState,
	eLightState,
	eCameraState,
	eTransformState,

	// Commands
	eSwapCommand,
	eDrawCommand,

#if defined(FAT_ENABLE_UNITTEST)
	eUnitTestCommand,
	eUnitTestCheckCommand,
	eUnitTestState,
#endif
FAT_ENUM_END()

}
