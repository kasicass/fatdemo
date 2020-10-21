#pragma once

#include "FatFramework/Driver/Client/ICommand.h"

namespace Fat {

class SwapCommandData : public ICommandData
{
public:
	SwapCommandData();

	virtual void Clear() override;
	virtual void Copy(const ICommandData* pData) override;
};

typedef TCommandWrapper<SwapCommandData, EFactoryObject::eSwapCommand> SwapCommand;

}
