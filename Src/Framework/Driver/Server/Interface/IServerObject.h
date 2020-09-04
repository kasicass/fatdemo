#pragma once

namespace Fat {

class IServerObject : public IntrusiveCounter, private NonCopyable
{
public:
	IServerObject() {}
	virtual ~IServerObject() {}
};

typedef TSmartPtr<IServerObject, TSmartPtrIntrusivePolicy> IServerObjectPtr;

}
