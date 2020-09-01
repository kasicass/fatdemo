#pragma once

#if defined(FAT_ENABLE_UNITTEST)

#define FatTest(x_Test)\
	do{\
		if ((x_Test) == false)\
		{\
			throw Fat::UnitTestFailureException();\
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
		throw Fat::UnitTestFailureException();\
	} while(false)

#else

#define FatTestAssert(...)

#endif

namespace Fat {

class UnitTestCase;
class IUnitTestManager
{
public:
	virtual void Run() = 0;

private:
	friend class UnitTestCase;
	virtual void RegisterTestCase(const UnitTestCase& testCase) = 0;
	virtual void NotifyEndedTest(const UnitTestCase& testCase, Bool succeded) = 0;
};
extern IUnitTestManager* theUnitTestMgr;

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

class UnitTestFailureException : public std::exception {};

}

#define TEST_DECLARE(TestFunction)\
	void TestFunction();\
	Fat::UnitTestCase _fatTestCase_##TestFunction(TestFunction, FAT_CONCAT(L, #TestFunction));\
	void TestFunction()

#define TEST_REGISTER(TestFunction)\
	extern Fat::UnitTestCase _fatTestCase_##TestFunction;\
	_fatTestCase_##TestFunction.RegisterMe()

#endif
