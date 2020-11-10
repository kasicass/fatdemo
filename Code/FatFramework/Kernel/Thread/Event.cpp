#include "FatFramework/Kernel/Thread/Event.h"

namespace Fat {

#if FAT_OS_WINDOWS

Event::Event()
{
	handle_ = ::CreateEventW(NULL, FALSE, FALSE, NULL);
}

Event::~Event()
{
	::CloseHandle(handle_);
}

void Event::Set()
{
	::SetEvent(handle_);
}

void Event::Wait() const
{
	::WaitForSingleObject(handle_, INFINITE);
}

bool Event::Wait(UInt32 timeoutMillis) const
{
	return (WaitForSingleObject(handle_, timeoutMillis) != WAIT_TIMEOUT);
}

#else

Event::Event()
{
	flag_ = false;
}

Event::~Event()
{
}

void Event::Set()
{
	lockNotify_.Lock();
	flag_ = true;
	cond_.Notify();
	lockNotify_.Unlock();
}

void Event::Wait() const
{
	lockNotify_.Lock();
	if (!flag_)
		cond_.Wait(lockNotify_);
	flag_ = false;
	lockNotify_.Unlock();
}

bool Event::Wait(UInt32 timeoutMillis) const
{
	bool result = true;
	lockNotify_.Lock();
	if (!flag_)
		result = cond_.TimedWait(lockNotify_, timeoutMillis);
	flag_ = false;
	lockNotify_.Unlock();
	return result;
}

#endif

}
