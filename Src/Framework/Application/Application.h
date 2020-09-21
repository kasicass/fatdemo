#pragma once

namespace Fat {

class Application : public IApplication
{
public:
	Application();
	virtual ~Application();

	virtual void Init() override;
	virtual void Shutdown() override;

	virtual void Update() override;
	virtual void Draw() override;

	virtual const wchar_t* GetTitle() const override;
	virtual IWidgetPtr GetWidget() const override;

#if !defined(FAT_OS_ANDROID)
	virtual void Run() override;
#endif

protected:
	void InitWidget();
	void ShutdownWidget();

private:
	IWidgetPtr pMainWnd_;
};

}
