#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/Common/Types.h"

#if defined(FAT_ENABLE_UNITTEST)

#define FatTest(x_Test)\
	do{\
		if ((x_Test) == false)\
		{\
			throw Fat::UnitTestFailureException(FAT_CONCAT(L,#x_Test), FAT_CONCAT(L,__FILE__), __LINE__);\
		}\
	} while(false)

#if defined(FAT_ENABLE_ASSERT)

#define FatTestAssert(x_Test)\
	do{\
		try\
		{\
			Fat::Assertion::UnitTestAssertionEnabler enabler;\
			x_Test;\
		}\
		catch (Fat::Assertion::UnitTestAssertionException&)\
		{\
			/* Assertion was produced so everything is fine */\
			break;\
		}\
		/* No assertion produced so report the failure */\
		throw Fat::UnitTestFailureException(FAT_CONCAT(L,#x_Test), FAT_CONCAT(L,__FILE__), __LINE__);\
	} while(false)

#else

#define FatTestAssert(...)

#endif

namespace Fat {

class UnitTestCase;
class UnitTestFailureException;

class IUnitTestManager
{
public:
	virtual void Run() = 0;
	virtual void NotifyTestFail(const UnitTestFailureException& e) = 0; // thread-safe

private:
	friend class UnitTestCase;
	virtual void RegisterTestCase(const UnitTestCase& testCase) = 0;
	virtual void NotifyEndedTest(const UnitTestCase& testCase, Bool succeded) = 0;
};
extern IUnitTestManager* GUnitTestMgr;

class UnitTestCase
{
public:
	typedef void (*TFuncType)();

	UnitTestCase(TFuncType pFunc, const wchar_t* name);
	~UnitTestCase();

	const wchar_t* GetName() const;
	void RegisterMe();

private:
	friend class UnitTestManager;
	void operator()();

	TFuncType pFunc_;
	const wchar_t* name_;
};

class UnitTestFailureException
{
public:
	UnitTestFailureException(const wchar_t* what, const wchar_t* file, int line);

	const wchar_t* What() const;
	const wchar_t* File() const;
	int Line() const;
	
private:
	const wchar_t* what_;
	const wchar_t* file_;
	int line_;
};

}

#define TEST_DECLARE(TestFunction)\
	void TestFunction();\
	Fat::UnitTestCase _fatTestCase_##TestFunction(TestFunction, FAT_CONCAT(L, #TestFunction));\
	void TestFunction()

#define TEST_REGISTER(TestFunction)\
	extern Fat::UnitTestCase _fatTestCase_##TestFunction;\
	_fatTestCase_##TestFunction.RegisterMe()

#endif
