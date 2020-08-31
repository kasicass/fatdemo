#pragma once

namespace Fat {

#if defined(FAT_USE_ASSERT)

#define FatAssert(condition, ...) \
	{\
		if ((condition) == false)\
		{\
			if (Fat::Assertion::FailureReport(__VA_ARGS__))\
			{\
				__debugbreak();\
			}\
		}\
	}

namespace Assertion
{
	Bool FailureReport(const wchar_t* format, ...);

#if defined(FAT_USE_UNITTEST)
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

#define FatAssertNoText(condition) FatAssert(condition, "Assertion failed")
#define FatAssertUnreachableCode() FatAssert(false, "Code should not be reached")
#define FatValidateNoText(condition) FatValidate(condition, "VAlidation failed")

}
