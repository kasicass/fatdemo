#include "FatFramework/Kernel/UnitTest/UnitTest.h"
#include "FatFramework/Kernel/Common/Time.h"
#include "FatFramework/Kernel/Common/Macros.h"
#include "FatFramework/Kernel/Common/Log.h"
#include "FatFramework/Kernel/Common/Memory.h"
#include <vector>

#if FAT_ENABLE_UNITTEST

namespace Fat {

//
// UnitTestManager
//

class UnitTestManager : public IUnitTestManager
{
public:
	virtual void Run() override;
	virtual void NotifyTestFail(const UnitTestFailureException& e) override;

private:
	virtual void RegisterTestCase(const UnitTestCase& testCase) override;
	virtual void NotifyEndedTest(const UnitTestCase& testCase, Bool succeded) override;

private:
	void Start();
	void RunInternal();
	void Finish();

	UInt32 suceededTestCount_;
	UInt32 failedTestCount_;
	Time startTime_;

	typedef std::vector<UnitTestCase> TestCaseArray;
	TestCaseArray* testCases_;
};

void UnitTestManager::Run()
{
	Start();
	RunInternal();
	Finish();
}

void UnitTestManager::Start()
{
	suceededTestCount_ = 0;
	failedTestCount_   = 0;
	startTime_ = Time::GetAppTime();
	FatLog(L"<UnitTest>: Starting");
}

void UnitTestManager::Finish()
{
	Time endTime = Time::GetAppTime();
	F32 elapsedMS = endTime.GetMilliSeconds() - startTime_.GetMilliSeconds();
	if (failedTestCount_ == 0)
	{
		FatLog(L"<UnitTest>: Ends with success, %u test were executed (%.0f ms)",
			suceededTestCount_, elapsedMS);
	}
	else
	{
		FatLog(L"<UnitTest>: Ends with %u failure(s), %u test were executed (%.0f ms)",
			failedTestCount_, suceededTestCount_ + failedTestCount_, elapsedMS);
	}

	if (testCases_ != NULL)
	{
		FatDelete(testCases_);
		testCases_ = NULL;
	}
}

void UnitTestManager::RunInternal()
{
	if (testCases_ == NULL)
		return;

	for (auto it = testCases_->begin(); it != testCases_->end(); ++it)
	{
		(*it)();
	}
}

void UnitTestManager::RegisterTestCase(const UnitTestCase& testCase)
{
	if (testCases_ == NULL)
		testCases_ = FatNew(TestCaseArray);
	testCases_->push_back(testCase);
}

void UnitTestManager::NotifyTestFail(const UnitTestFailureException& e)
{
	FatLog(L" - \"%ls\" fails at (%ls:%d)", e.What(), e.File(), e.Line());
}

void UnitTestManager::NotifyEndedTest(const UnitTestCase& testCase, Bool succeded)
{
	FatLog(L" - test %02u (%ls) : %ls",
		suceededTestCount_ + failedTestCount_,
		testCase.GetName(),
		succeded ? L"OK" : L"FAILED");

	succeded ? ++suceededTestCount_ : ++failedTestCount_;
}

//
// IUnitTestManager Singleton
//

static UnitTestManager myUnitTestMgr;
IUnitTestManager* GUnitTestMgr = &myUnitTestMgr;

//
// UnitTestCase
//

UnitTestCase::UnitTestCase(TFuncType pFunc, const wchar_t* name) :
	pFunc_(pFunc),
	name_(name)
{
}

UnitTestCase::~UnitTestCase()
{
}

const wchar_t* UnitTestCase::GetName() const
{
	return name_;
}

void UnitTestCase::RegisterMe()
{
	GUnitTestMgr->RegisterTestCase(*this);
}

void UnitTestCase::operator()()
{
	Bool succeed = true;

	try
	{
		(*pFunc_)();
	}
	catch (UnitTestFailureException& e)
	{
		succeed = false;
		GUnitTestMgr->NotifyTestFail(e);
	}

	GUnitTestMgr->NotifyEndedTest(*this, succeed);
}

//
// UnitTestFailureException
//

UnitTestFailureException::UnitTestFailureException(const wchar_t* what, const wchar_t* file, int line) :
	what_(what),
	file_(file),
	line_(line)
{
}

const wchar_t* UnitTestFailureException::What() const
{
	return what_;
}

const wchar_t* UnitTestFailureException::File() const
{
	return file_;
}

int UnitTestFailureException::Line() const
{
	return line_;
}

}

#endif
