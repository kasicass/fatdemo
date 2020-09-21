#pragma once

namespace Fat {

struct IWidget : public IntrusiveCounter
{
	virtual ~IWidget() {}

	virtual void* GetHandle() const = 0;
};

typedef TSmartPtr<IWidget, TSmartPtrIntrusivePolicy> IWidgetPtr;
IWidgetPtr CreateWidget(const wchar_t* title, UInt32 width, UInt32 height);

}
