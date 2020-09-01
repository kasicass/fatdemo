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
		throw UnitTestFailureException();\
	} while(false)

#else

#define FatTestAssert(...)

#endif

namespace Fat {

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

private:
	friend class UnitTestManager;
	virtual void operator()();

	TFuncType pFunc_;
	const wchar_t* name_;
};

class UnitTestFailureException : public std::exception {};

}

#define TEST(TestFunction)\
	void TestFunction();\
	static Fat::UnitTestCase _fatTestCase_##TestFunction(TestFunction, L#TestFunction);\
	void TestFunction()

#endif
