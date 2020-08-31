#pragma once

namespace Fat {

struct IApplication
{
	virtual ~IApplication() {}

	virtual void Init() = 0;
	virtual void Run() = 0;
	virtual void Shutdown() = 0;
};
extern IApplication* theApp;

}
