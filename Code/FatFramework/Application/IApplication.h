#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/Common/Types.h"

namespace Fat {

struct IWidget;
struct IApplication
{
	virtual ~IApplication() {}

	virtual void Init() = 0;
	virtual void Shutdown() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Resize(UInt32 width, UInt32 height) = 0;

	virtual const wchar_t* GetTitle() const = 0;
	virtual IWidget* GetMainWnd() const = 0;

#if !defined(FAT_OS_ANDROID)
	virtual void Run() = 0;
#endif
};
extern IApplication* theApp;

}

#define FAT_APP_DEFINE(ClassName)\
	static ClassName myApp;\
	Fat::IApplication* Fat::theApp = &myApp;

