#include "FatFramework.h"

namespace Fat {

RenderTargetStateData::RenderTargetStateData()
{
	Clear();
}

RenderTargetStateData::~RenderTargetStateData()
{
}

void RenderTargetStateData::Clear()
{
	width_  = 0;
	height_ = 0;
	color_  = Float4(0, 0, 0, 0);
}

void RenderTargetStateData::Copy(const ICommandData* pDataIn)
{
	const RenderTargetStateData* pData = (const RenderTargetStateData*)pDataIn;
	width_  = pData->width_;
	height_ = pData->height_;
	color_  = pData->color_;
}

UInt32 RenderTargetStateData::GetWidth() const
{
	return width_;
}

void RenderTargetStateData::SetWidth(UInt32 width)
{
	width_ = width;
}

UInt32 RenderTargetStateData::GetHeight() const
{
	return height_;
}

void RenderTargetStateData::SetHeight(UInt32 height)
{
	height_ = height;
}

const Float4& RenderTargetStateData::GetColor() const
{
	return color_;
}

void RenderTargetStateData::SetColor(const Float4& color)
{
	color_ = color;
}

}
