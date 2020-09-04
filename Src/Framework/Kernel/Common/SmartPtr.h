#pragma once

namespace Fat {

// Smart pointer implementation. This very simple policy based design allows to use this template
// implementation with various ownerships rules: Windows COM, intrusive counters, ...
template <typename T, template<typename> class Policy>
class TSmartPtr : private Policy<T>
{
public:
	typedef Policy<T> _MyPolicy;

	TSmartPtr(T* pObject = NULL) : 
		pObject_(NULL)
	{
		Assign(pObject);
	}

	TSmartPtr(const TSmartPtr& rhs) :
		pObject_(NULL)
	{
		Assign(rhs.pObject_);
	}

	template <typename U, template<typename> class OtherPolicy>
	TSmartPtr(const TSmartPtr<U, OtherPolicy>& rhs) :
		pObject_(NULL)
	{
		Assign(rhs.pObject_);
	}

	~TSmartPtr()
	{
		Assign((T*)NULL);
	}

	void operator=(const TSmartPtr& rhs)
	{
		Assign(rhs.pObject_);
	}

	template <typename U, template<typename> class OtherPolicy>
	void operator=(const TSmartPtr<U, OtherPolicy>& rhs)
	{
		Assign(rhs.pObject_);
	}

	void operator=(T* pObject)
	{
		Assign(pObject);
	}

	T* Get() const
	{
		FatAssert(pObject_ != NULL, L"Dereferencing NULL pointer");
		return pObject_;
	}

	T* GetUnsafe() const
	{
		return pObject_;
	}

	T& operator*() const
	{
		FatAssert(pObject_ != NULL, L"Dereferencing NULL pointer");
		return *pObject_;
	}

	T* operator->() const
	{
		FatAssert(pObject_ != NULL, L"Dereferencing NULL pointer");
		return pObject_;
	}

	template <typename U, template<typename> class OtherPolicy>
	Bool operator==(const TSmartPtr<U, OtherPolicy>& rhs)
	{
		return pObject_ == rhs.pObject_;
	}

	template <typename U, template<typename> class OtherPolicy>
	Bool operator!=(const TSmartPtr<U, OtherPolicy>& rhs)
	{
		return pObject_ != rhs.pObject_;
	}

private:
	// Internal assigment function used by assignment operators
	template <typename OtherObject>
	void Assign(OtherObject* pOtherObject)
	{
		if (pObject_ == pOtherObject)
			return;

		if (pObject_) _MyPolicy::OnRelease(pObject_);
		pObject_ = pOtherObject;
		if (pObject_) _MyPolicy::OnAcquire(pObject_);
	}

	// TSmartPtr for any other type is friend to allow copies
	template <typename U, template<typename> class OtherPolicy>
	friend class TSmartPtr;

	T* pObject_;
};

// Smart pointer empty polity. Does nothing but letting TSmartPtr doing its assertions on dereferencing
template <typename T>
class TSmartPtrAssertPolicy
{
protected:
	TSmartPtrAssertPolicy() {}
	~TSmartPtrAssertPolicy() {}

	void OnAcquire(T* obj) {}
	void OnRelease(T* obj) {}
};

// Smart pointer COM policy
template <typename T>
class TSmartPtrCOMPolicy
{
protected:
	TSmartPtrCOMPolicy() {}
	~TSmartPtrCOMPolicy() {}

	void OnAcquire(T* pObject)
	{
		pObject->AddRef();
	}

	void OnRelease(T* pObject)
	{
		pObject->Release();
	}
};

// Smart pointer intrusive reference counter object
class IntrusiveCounter
{
public:
	Int32 GetRefCount()
	{
		return refCount_;
	}

protected:

	IntrusiveCounter(Int32 initCount = 0) :
		refCount_(initCount)
	{
		FatAssert(initCount >= 0, L"Count should never be negative");
	}

	~IntrusiveCounter()
	{
	}

	template <typename T>
	friend class TSmartPtrIntrusivePolicy;

	Int32 AddRef()
	{
		Int32 ret = ++refCount_;
		FatAssert(ret > 0, L"Count should never be negative");
		return ret;
	}

	Int32 DecRef()
	{
		Int32 ret = --refCount_;
		FatAssert(ret >= 0, L"Count should never be negative");
		return ret;
	}

	void operator=(IntrusiveCounter& rhs)
	{
		refCount_ = 0;
	}

private:
	AtomicInt refCount_;
};

// Smart pointer intrusive reference counted policy
template <typename T>
class TSmartPtrIntrusivePolicy
{
protected:
	TSmartPtrIntrusivePolicy() {}
	~TSmartPtrIntrusivePolicy() {}

	void OnAcquire(T* pObject)
	{
		pObject->AddRef();
	}

	void OnRelease(T* pObject)
	{
		if (pObject->DecRef() == 0)
		{
			FatDelete(pObject);
		}
	}
};

}
