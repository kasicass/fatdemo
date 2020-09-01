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

#define FAT_APP_DEFINE(ClassName)\
	static ClassName myApp;\
	Fat::IApplication* Fat::theApp = &myApp;

