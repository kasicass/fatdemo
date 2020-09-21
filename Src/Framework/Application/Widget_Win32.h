#pragma once

#if defined(FAT_OS_WINDOWS)

namespace Fat {

class WidgetWin32 final : public IWidget
{
public:
	WidgetWin32(const wchar_t* title, UInt32 width, UInt32 height);
	virtual ~WidgetWin32();

	virtual void* GetHandle() const override;
	virtual Bool IsActive() const override;

protected:
	bool InitWindowClass();
	void ShutdownWindowClass();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool InitWindow();
	void OnActive(Bool value);

private:
	HWND hWnd_;
	UInt32 width_;
	UInt32 height_;
	const wchar_t* title_;
	Bool active_;
};

}

#endif
