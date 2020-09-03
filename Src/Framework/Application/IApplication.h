#pragma once

namespace Fat {

struct IApplication
{
	virtual ~IApplication() {}

	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void Update() = 0;
	virtual void Draw() = 0;

#if !defined(FAT_OS_ANDROID)
	virtual void Run() = 0;
#endif
};
extern IApplication* theApp;

}

#define FAT_APP_DEFINE(ClassName)\
	static ClassName myApp;\
	Fat::IApplication* Fat::theApp = &myApp;

