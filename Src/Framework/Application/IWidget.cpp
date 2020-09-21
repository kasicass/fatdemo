#include "FatFramework.h"

namespace Fat {

IWidgetPtr CreateWidget(const wchar_t* title, UInt32 width, UInt32 height)
{
	return new WidgetWin32(title, width, height);
}

}
