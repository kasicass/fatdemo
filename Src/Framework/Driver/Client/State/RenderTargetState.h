#pragma once

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



private:
	UInt32 width_;
	UInt32 height_;
	// Float4 color_;
};

}
