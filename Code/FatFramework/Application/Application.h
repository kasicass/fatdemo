#pragma once

#include "FatFramework/Application/IApplication.h"

namespace Fat {

class Application : public IApplication
{
public:
	Application();
	virtual ~Application();

	virtual void Init() override;
	virtual void Shutdown() override;

	virtual void Update() override;
	virtual void Render() override;
	virtual void Resize(UInt32 width, UInt32 height) override;

	virtual const wchar_t* GetTitle() const override;
	virtual IWidget* GetMainWnd() const override;

#if !defined(FAT_OS_ANDROID)
	virtual void Run() override;
#endif

protected:
	void InitWidget(UInt32 width, UInt32 height);
	void ShutdownWidget();

private:
	IWidget* pMainWnd_;

};

}
