#pragma once

#if defined(FAT_OS_WINDOWS)

namespace Fat {

class D3D9Factory : public IServerFactory
{
public:
	virtual void Init() override;
	virtual void Shutdown() override;

	virtual IServerObject* Instantiate(EFactoryObject::EValue value) override;
};
extern IServerFactory* theD3D9Factory;

}

#endif
