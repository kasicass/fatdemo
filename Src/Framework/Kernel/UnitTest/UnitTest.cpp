#include "FatFramework.h"

#if defined(FAT_ENABLE_UNITTEST)

namespace Fat {

//
// UnitTestManager
//

class UnitTestManager : public IUnitTestManager
{
public:
	virtual void Run() override;

private:
	virtual void RegisterTestCase(const UnitTestCase& testCase) override;
	virtual void NotifyEndedTest(const UnitTestCase& testCase, Bool succeded) override;

private:
	void Start();
	void RunInternal();
	void Finish();

	UInt32 suceededTestCount_;
	UInt32 failedTestCount_;

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
	FatLog(L"<UnitTest>: Starting");
}

void UnitTestManager::Finish()
{
	if (failedTestCount_ == 0)
	{
		FatLog(L"<UnitTest>: Ends with success, %u test were executed",
			suceededTestCount_);
	}
	else
	{
		FatLog(L"<UnitTest>: Ends with %u failure(s), %u test were executed",
			failedTestCount_, suceededTestCount_ + failedTestCount_);
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

void UnitTestManager::NotifyEndedTest(const UnitTestCase& testCase, Bool succeded)
{
	FatLog(L" - test %u (%ls) : %ls",
		suceededTestCount_ + failedTestCount_,
		testCase.GetName(),
		succeded ? L"OK" : L"FAILED");

	succeded ? ++suceededTestCount_ : ++failedTestCount_;
}

//
// IUnitTestManager Singleton
//

static UnitTestManager myUnitTestMgr;
IUnitTestManager* theUnitTestMgr = &myUnitTestMgr;

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
	theUnitTestMgr->RegisterTestCase(*this);
}

void UnitTestCase::operator()()
{
	Bool succeed = true;

	try
	{
		(*pFunc_)();
	}
	catch (UnitTestFailureException&)
	{
		succeed = false;
	}

	theUnitTestMgr->NotifyEndedTest(*this, succeed);
}

}

#endif
