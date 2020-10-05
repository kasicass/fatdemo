#pragma once

#include "Driver/Client/ICommand.h"

namespace Fat {

class DrawCommandData : public ICommandData
{
public:
	DrawCommandData();

	virtual void Clear() override;
	virtual void Copy(const ICommandData* pData) override;
};

typedef TCommandWrapper<DrawCommandData, EFactoryObject::eDrawCommand> DrawCommand;

}
