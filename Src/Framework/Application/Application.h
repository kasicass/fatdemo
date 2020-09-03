#pragma once

namespace Fat {

class Application : public IApplication
{
public:
	Application();
	virtual ~Application();

	virtual void Init() override;
	virtual void Shutdown() override;

	virtual void Update() override;
	virtual void Draw() override;

#if !defined(FAT_OS_ANDROID)
	virtual void Run() override;
#endif
};

}
