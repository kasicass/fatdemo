#pragma once

namespace Fat {

class IObject : private NonCopyable
{
public:
	IObject(EFactoryObject::EValue value);
	virtual ~IObject();

protected:
	IServerObject* GetServerObject() const;

private:
	IServerObjectPtr pServerObject_;
};

}
