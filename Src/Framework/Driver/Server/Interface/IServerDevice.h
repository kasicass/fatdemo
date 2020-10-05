#pragma once

#include "Driver/Server/Interface/IServerObject.h"

namespace Fat {

class IServerDevice : public IServerObject
{
public:
	IServerDevice();
	virtual ~IServerDevice();

	void ResetNeeded();	// Request a reset

protected:
	void Reseted();		// Flag the reset as done

private:
	friend class Device;

	Bool IsResetNeeded() const;
	virtual Bool Reset();

private:
	// Stores if the reset has been asked using an atomic integer as reset flag is tested
	// and written from the client and the server threads
	AtomicInt resetNeeded_;
};

}
