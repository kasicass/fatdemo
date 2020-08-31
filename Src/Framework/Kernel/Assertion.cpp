#include "FatFramework.h"
#include <Windows.h>

#if defined(FAT_USE_ASSERT)

namespace Fat { namespace Assertion {

#if defined(FAT_USE_UNITTEST)
FAT_THREAD_LOCAL Bool UnitTestAssertionEnabler::s_isEnabled = false;
#endif

Bool FailureReport(const wchar_t* format, ...)
{
#if defined(FAT_USE_UNITTEST)
	if (UnitTestAssertionEnabler::IsEnabled())
	{
		throw UnitTestAssertionException();
	}
#endif

	va_list args;
	va_start(args, format);

	StackWString msg;
	

	va_end(args);
}

}}

#endif
