#pragma once

namespace Fat {

class Application : public IApplication
{
public:
	Application();
	virtual ~Application();

	virtual void Init() override;
	virtual void Shutdown() override;
};

}
