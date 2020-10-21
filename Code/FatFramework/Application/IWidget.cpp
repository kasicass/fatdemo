#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/Common/Memory.h"
#include "FatFramework/Application/IWidget.h"
#include "FatFramework/Application/Widget_Win32.h"

namespace Fat {

IWidget* CreateWidget(const wchar_t* title, UInt32 width, UInt32 height)
{
#if defined(FAT_OS_WINDOWS)
	return FatNew(WidgetWin32, title, width, height);
#else
	return NULL;
#endif
}

}
