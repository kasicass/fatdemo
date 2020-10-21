#pragma once

#include "FatFramework/Kernel/Math/Float4.h"
#include "FatFramework/Driver/Client/IState.h"

namespace Fat {

// Render target state command data. Embeds render target sizes and clear color. This clear color information
// might not be a real-world case but rather demonstrates the ont-to-many abstraction design.
class RenderTargetStateData : public IStateData
{
public:
	RenderTargetStateData();
	virtual ~RenderTargetStateData();

	virtual void Clear() override;
	virtual void Copy(const ICommandData* pData) override;

	UInt32 GetWidth() const;
	void SetWidth(UInt32 width);

	UInt32 GetHeight() const;
	void SetHeight(UInt32 height);

	const Float4& GetColor() const;
	void SetColor(const Float4& color);

private:
	UInt32 width_;
	UInt32 height_;
	Float4 color_;
};

typedef TStateWrapper<RenderTargetStateData,
	EStateType::eRenderTarget,
	EFactoryObject::eRenderTargetState> RenderTargetState;

}
