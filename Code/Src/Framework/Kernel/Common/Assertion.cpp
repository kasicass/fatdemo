#include "Kernel/Common/Assertion.h"
#include "Kernel/String/TStackString.h"
#include "Kernel/Common/Log.h"

#if defined(FAT_ENABLE_ASSERT)

namespace Fat { namespace Assertion {

#if defined(FAT_ENABLE_UNITTEST)
FAT_THREAD_LOCAL Bool UnitTestAssertionEnabler::s_isEnabled = false;
#endif

Bool FailureReport(const wchar_t* format, ...)
{
#if defined(FAT_ENABLE_UNITTEST)
	if (UnitTestAssertionEnabler::IsEnabled())
	{
		throw UnitTestAssertionException();
	}
#endif

	StackWString msg;

	va_list argList;
	va_start(argList, format);
	msg.FormatV(format, argList);
	va_end(argList);

	FatLog(msg.c_str());

#if defined(FAT_OS_WINDOWS)
	Int32 n = MessageBox(NULL, msg.c_str(), L"FatFramework: Assertion failed", MB_ABORTRETRYIGNORE);
	switch (n)
	{
	case IDABORT: exit(EXIT_FAILURE);  // Exit appliation
	case IDIGNORE: return false;       // Continue execution
	default: return true;              // Generate a breakpoint
	}
#else
	return true;
#endif
}

}}

#endif