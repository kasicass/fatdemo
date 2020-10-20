#pragma once

namespace Fat {

template <typename T>
class TAutoLock
{
public:
	TAutoLock(T& lock) : lock_(lock) { lock_.Lock(); }
	~TAutoLock() { lock_.Unlock(); }

private:
	TAutoLock();
	TAutoLock(const TAutoLock<T>&);
	TAutoLock<T>& operator=(const TAutoLock<T>&);

private:
	T& lock_;
};

}
