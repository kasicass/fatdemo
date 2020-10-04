#pragma once

#include "Kernel/PlatformConfig.h"
#include "Kernel/PlatformHeaders.h"
#include "Kernel/Common/Types.h"
#include "Kernel/Common/Macros.h"
#include <exception>

namespace Fat {

#if defined(FAT_ENABLE_ASSERT)

namespace Assertion
{
	Bool FailureReport(const wchar_t* format, ...);

#if defined(FAT_ENABLE_UNITTEST)
	// RAII object that enable UnitTest assertion report rather than the default report
	class UnitTestAssertionEnabler
	{
	public:
		UnitTestAssertionEnabler()
		{
			s_isEnabled = true;
		}

		~UnitTestAssertionEnabler()
		{
			s_isEnabled = false;
		}

		static Bool IsEnabled()
		{
			return s_isEnabled;
		}

	private:
		static FAT_THREAD_LOCAL Bool s_isEnabled;
	};

	class UnitTestAssertionException : public std::exception {};
#endif
}

#define FatAssert(condition, ...) \
	{\
		if ((condition) == false)\
		{\
			if (Fat::Assertion::FailureReport(__VA_ARGS__))\
			{\
				fat_debugbreak();\
			}\
		}\
	}

#define FatIfBuildAssertion(...) __VA_ARGS__

#else

#define FatAssert(condition, ...)
#define FatIfBuildAssertion(...)

#endif

// Unlike assertions, condition execution is maintained for all build modes
#define FatValidate(condition, ...) \
	{\
		FatIfBuildAssertion(Bool ok =)(condition);\
		FatAssert(ok, __VA_ARGS__);\
	}

#define FatAssertNoText(condition) FatAssert(condition, L"Assertion failed(%ls:%d)", FAT_CONCAT(L,__FILE__), __LINE__)
#define FatAssertUnreachableCode() FatAssert(false, L"Code should not be reached(%ls:%d)", FAT_CONCAT(L,__FILE__), __LINE__)
#define FatValidateNoText(condition) FatValidate(condition, L"Validation failed(%ls:%d)", FAT_CONCAT(L,__FILE__), __LINE__)

}
