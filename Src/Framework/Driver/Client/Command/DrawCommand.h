#pragma once

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
