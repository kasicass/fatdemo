#pragma once

namespace Fat {

class Thread;
typedef TSmartPtr<Thread, TSmartPtrIntrusivePolicy> ThreadPtr;

// Thread object implementation. At least one reference should be maintained on a created thread (using
// a ThreadPtr) in order for it to stay alive. Furthermore, a thread could not be create until it has been
// joined
class Thread : public IntrusiveCounter, private NonCopyable
{
public:
	typedef void (*TThreadFunc)(void*);

	// Create anew thread that we'll execute pFunc
	static ThreadPtr Create(const char* name, TThreadFunc pFunc, void* args);

	// Attach a Thread object to an existing os thread. Only one Thread could be attached at a time to
	// an os thread
	static ThreadPtr Attach(const char* name);
	static void InitMainThread();
	static void ShutdownMainThread();

	// Sleep the calling thread for milliSeconds
	static void Sleep(UInt32 milliSeconds);

	// Get current thread object
	static ThreadPtr GetCurrent();

private:
	// A Thread pointer in thread local storage to store calling thread Thread object
	static FAT_THREAD_LOCAL Thread* s_pCurrentThread;

public:
	~Thread();

	void Run();
	void Join();

	Bool operator==(const Thread& rhs) const;
	Bool operator!=(const Thread& rhs) const;

private:
	// Constructors are private to ensure proper public function (Attach/Create) are used

	Thread(const char* name);
	Thread(const char* name, TThreadFunc pFunc, void* args);

	const char* GetName() const
	{
		return name_.c_str();
	}

private:

#if defined(FAT_OS_WINDOWS)
	static unsigned __stdcall RunThis(void *thisPtr);
#else
	static void* RunThis(void *thisPtr);
#endif

#if defined(FAT_OS_WINDOWS)
	uintptr_t         thrHandle_; 
#else
	pthread_t         thrHandle_;
#endif

	ThreadId          thrId_;
	volatile bool     isRunning_;

	MutexFast         exitMutex_; // Mutex used to safeguard thread exit condition signaling
	ConditionVariable exitCond_;  // Signaled when the thread si about to exit

	TThreadFunc       pFunc_;
	void*             args_;
	StackString32     name_;
};

}
