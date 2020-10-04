#include "Kernel/Common/Log.h"
#include "Kernel/Common/OS.h"
#include "Kernel/String/TStackString.h"

#if defined(FAT_ENABLE_LOG)

namespace Fat {

void LogPrintf(const wchar_t* format, ...)
{
	StackWString msg;

	va_list argList;
	va_start(argList, format);
	msg.FormatV(format, argList);
	va_end(argList);
		
	OS::Println(msg.c_str());
}

}

#endif
