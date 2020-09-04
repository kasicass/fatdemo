#pragma once

namespace Fat {

// Driver factory enum
FAT_ENUM_BEGIN(EGraphicAPI)
	eD3D9,
	eD3D11,
	eOpenGL,
	eUnitTest,
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


class IServerObject;
class IServerFactory;

// This minimal factory mechanism allow to instantiate a server object based on object type (EFactoryObject)
// and the selected graphic API (EGraphicAPI)
class IFactorySelector
{
public:
	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void SelectorFactory(EGraphicAPI::EValue api) = 0;
	virtual void UnselectFactory() = 0;

private:
	// IObject will call Instantiate()
	// Instantiate a server object form a client one (using client name as the identifier)
	friend class IObject;
	virtual IServerObject* Instantiate(EFactoryObject::EValue value) = 0;

	// Register a factory object for api
	friend class IServerFactory;
	virtual void RegisterFactory(EGraphicAPI::EValue api, IServerFactory* factory) = 0;
};
extern IFactorySelector* theFactorySelector;

}
