#include "FatFramework/Kernel/Thread/Thread.h"
#include "FatFramework/Kernel/Common/Log.h"
#include "FatFramework/Kernel/Common/Memory.h"
#include "FatFramework/Kernel/UnitTest/UnitTest.h"

namespace Fat {

//
// static methods
//

static ThreadPtr s_pMainThread = NULL;
FAT_THREAD_LOCAL Thread* Thread::s_pCurrentThread = NULL;

ThreadPtr Thread::Attach(const char* name)
{
	FatAssert(Thread::s_pCurrentThread == NULL, L"This thread is already attached to a Thread object");

	ThreadPtr pThread = FatNew(Thread, name);
	FatValidate(pThread->DecRef() >= 1, L"Invalid ref count while creating a thread");

	return pThread;
}

void Thread::InitMainThread()
{
	FatLog(L"<Thread>: Init");
	s_pMainThread = Thread::Attach("MainThread");
}

void Thread::ShutdownMainThread()
{
	s_pCurrentThread = NULL;
	s_pMainThread = NULL;
	FatLog(L"<Thread>: Shutdown");
}

ThreadPtr Thread::GetCurrent()
{
	FatAssert(s_pCurrentThread != NULL, L"Invalid function call, no Thread object associated to that thread");
	return s_pCurrentThread;
}

ThreadPtr Thread::Create(const char* name, TThreadFunc pFunc, void* args)
{
	FatAssert(pFunc != NULL, L"Invalid thread function");

	ThreadPtr pThread = FatNew(Thread, name, pFunc, args);
	FatValidate(pThread->DecRef() >= 1, L"Invalid ref count while creating a thread");

	return pThread;
}

#if defined(FAT_OS_WINDOWS)

#if !defined(FAT_RELEASE_BUILD)
static void SetThreadName_SEH(DWORD threadId, const char* threadName)
{
	const DWORD MS_VC_EXCEPTION = 0x406D1388;

	struct SThreadNameDesc
	{
		DWORD  dwType;      // Must be 0x1000.
		LPCSTR szName;      // Pointer to name (in user addr space).
		DWORD  dwThreadID;  // Thread ID (-1=caller thread).
		DWORD  dwFlags;     // Reserved for future use, must be zero.
	};

	SThreadNameDesc info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = threadId;
	info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable : 6312 6322)
	// warning C6312: Possible infinite loop: use of the constant EXCEPTION_CONTINUE_EXECUTION in the exception-filter expression of a try-except
	// warning C6322: empty _except block
	__try
	{
		// Raise exception to set thread name for attached debugger
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(DWORD), (ULONG_PTR*)&info);
	}
	__except (GetExceptionCode() == MS_VC_EXCEPTION ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_EXECUTE_HANDLER)
	{
	}
#pragma warning(pop)
}
#endif

void Thread::Sleep(UInt32 milliSeconds)
{
	::Sleep(milliSeconds);
}

Thread::Thread(const char* name) :
	IntrusiveCounter(1),
	thrHandle_(0),
	thrId_(THREADID_NULL),
	isRunning_(true),
	pFunc_(NULL),
	args_(NULL),
	name_(name)
{
	thrId_ = ::GetCurrentThreadId();

	FatAssertNoText(Thread::s_pCurrentThread == NULL);
	s_pCurrentThread = this;

#if !defined(FAT_RELEASE_BUILD)
	SetThreadName_SEH(thrId_, name);
#endif
}

Thread::Thread(const char* name, TThreadFunc pFunc, void* args) :
	IntrusiveCounter(1),
	thrHandle_(0),
	thrId_(THREADID_NULL),
	isRunning_(false),
	pFunc_(pFunc),
	args_(args),
	name_(name)
{
	thrHandle_ = _beginthreadex(NULL, 0, RunThis, this, 0, (unsigned int*)&thrId_);
	FatAssert(thrHandle_ != NULL, L"Thread creation fails");
	isRunning_ = (thrHandle_ != NULL);
}

Thread::~Thread()
{
}

void Thread::Run()
{
#if defined(FAT_ENABLE_UNITTEST)
	try
	{
		(*pFunc_)(args_);
	}
	catch (UnitTestFailureException& e)
	{
		GUnitTestMgr->NotifyTestFail(e);
	}
#else
	(*pFunc_)(args_);
#endif

}

void Thread::Join()
{
	exitMutex_.Lock();
	while (isRunning_)
	{
		exitCond_.Wait(exitMutex_);
	}
	exitMutex_.Unlock();
}

Bool Thread::operator==(const Thread& rhs) const
{
	return thrId_ == rhs.thrId_;
}

Bool Thread::operator!=(const Thread& rhs) const
{
	return thrId_ != rhs.thrId_;
}

unsigned __stdcall Thread::RunThis(void *thisPtr)
{
	Thread* self = (Thread*)thisPtr;

	FatAssertNoText(Thread::s_pCurrentThread == NULL);
	Thread::s_pCurrentThread = self;

#if !defined(FAT_RELEASE_BUILD)
	SetThreadName_SEH(self->thrId_, self->name_.c_str());
#endif

	self->Run();

	FatAssertNoText(Thread::s_pCurrentThread != NULL);
	Thread::s_pCurrentThread = NULL;

	self->exitMutex_.Lock();
	self->isRunning_ = false;
	self->exitCond_.NotifyAll();
	self->exitMutex_.Unlock();

	// Note on: ExitThread() (from MSDN)
	// ExitThread is the preferred method of exiting a thread in C code.
	// However, in C++ code, the thread is exited before any destructor can be called or any other automatic cleanup can be performed.
	// Therefore, in C++ code, you should return from your thread function.
	return 0;
}

#else

Thread::Thread(const char* name) :
	IntrusiveCounter(1),
	thrHandle_(0),
	thrId_(THREADID_NULL),
	isRunning_(true),
	pFunc_(NULL),
	args_(NULL),
	name_(name)
{
	thrId_ = (ThreadId)pthread_self();

	FatAssertNoText(Thread::s_pCurrentThread == NULL);
	s_pCurrentThread = this;
}

Thread::Thread(const char* name, TThreadFunc pFunc, void* args) :
	IntrusiveCounter(1),
	thrHandle_(0),
	thrId_(THREADID_NULL),
	isRunning_(false),
	pFunc_(pFunc),
	args_(args),
	name_(name)
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	int ret = pthread_create(&thrHandle_, &attr, RunThis, this);
	FatAssert(ret == 0, L"Thread creation fails");
	isRunning_ = (ret == 0);
	pthread_attr_destroy(&attr);
}

void Thread::Sleep(UInt32 milliSeconds)
{
	struct timespec sleepTime;
	sleepTime.tv_sec  = (milliSeconds / 1000);
	sleepTime.tv_nsec = (milliSeconds % 1000)*1000000;

	::nanosleep(&sleepTime, NULL);
}

Thread::~Thread()
{
}

void Thread::Run()
{
#if defined(FAT_ENABLE_UNITTEST)
	try
	{
		(*pFunc_)(args_);
	}
	catch (UnitTestFailureException& e)
	{
		GUnitTestMgr->NotifyTestFail(e);
	}
#else
	(*pFunc_)(args_);
#endif

}

void Thread::Join()
{
	exitMutex_.Lock();
	while (isRunning_)
	{
		exitCond_.Wait(exitMutex_);
	}
	exitMutex_.Unlock();
}

Bool Thread::operator==(const Thread& rhs) const
{
	return pthread_equal(thrId_, rhs.thrId_);
}

Bool Thread::operator!=(const Thread& rhs) const
{
	return !pthread_equal(thrId_, rhs.thrId_);
}

void* Thread::RunThis(void *thisPtr)
{
	Thread* self = (Thread*)thisPtr;
	self->thrId_ = (ThreadId)pthread_self();

	FatAssertNoText(Thread::s_pCurrentThread == NULL);
	Thread::s_pCurrentThread = self;

	self->Run();

	FatAssertNoText(Thread::s_pCurrentThread != NULL);
	Thread::s_pCurrentThread = NULL;

	self->exitMutex_.Lock();
	self->isRunning_ = false;
	self->exitCond_.NotifyAll();
	self->exitMutex_.Unlock();

	return 0;
}

#endif

}
