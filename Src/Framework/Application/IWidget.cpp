#include "FatFramework.h"

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
