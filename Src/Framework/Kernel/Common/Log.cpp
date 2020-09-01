#include "FatFramework.h"

#if defined(FAT_ENABLE_LOG)

namespace Fat {

void LogPrintf(const wchar_t* format, ...)
{
	StackWString tmp;

	va_list argList;
	va_start(argList, format);
	tmp.FormatV(format, argList);
	va_end(argList);

	StackWString msg = L"[FAT] ";
	msg += tmp;
	msg += L'\n';

#if defined(FAT_OS_WINDOWS)
	OutputDebugString(msg.c_str());
#endif

	wprintf(msg.c_str());
}

}

#endif
