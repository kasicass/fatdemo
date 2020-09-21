#pragma once

namespace Fat {

struct IWidget
{
	virtual ~IWidget() {}

	virtual void* GetHandle() const = 0;
	virtual Bool IsActive() const = 0;
};

IWidget* CreateWidget(const wchar_t* title, UInt32 width, UInt32 height);

}
