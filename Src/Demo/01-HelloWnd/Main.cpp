#include "FatFramework.h"

using namespace Fat;

class HelloWndApp : public Application
{
public:
	HelloWndApp();
	virtual ~HelloWndApp();

	virtual void Init() override;
	virtual void Shutdown() override;

	virtual void Update() override;

#if !defined(FAT_OS_ANDROID)
	virtual void Run() override;
#endif

protected:
	void SetupRenderTarget();

private:
	Device* pDevice_;
	Context* pContext_;
	RenderTargetState* pRenderTargetState_;
	SwapCommand* pSwapCommand_;
};
FAT_APP_DEFINE(HelloWndApp)

HelloWndApp::HelloWndApp()
{
	pDevice_ = NULL;
	pContext_ = NULL;
	pRenderTargetState_ = NULL;
	pSwapCommand_ = NULL;
}

HelloWndApp::~HelloWndApp()
{

}

void HelloWndApp::Init()
{
	Application::Init();

	theFactorySelector->SelectorFactory(EGraphicAPI::eD3D9);
	pDevice_ = FatNew(Device);
	pContext_ = FatNew(Context, *pDevice_);
	pRenderTargetState_ = FatNew(RenderTargetState);
	pSwapCommand_ = FatNew(SwapCommand);
}

void HelloWndApp::Shutdown()
{
	FAT_SAFE_DELETE(pSwapCommand_);
	FAT_SAFE_DELETE(pRenderTargetState_);
	FAT_SAFE_DELETE(pContext_);
	FAT_SAFE_DELETE(pDevice_);
	Application::Shutdown();
}

void HelloWndApp::Update()
{
	if (pContext_->Begin())
	{
		pContext_->Bind(*pRenderTargetState_);

		pContext_->Queue(pSwapCommand_);

		pContext_->End();
	}

#if 0
	// Begin the rendering context. Begin can fail if the server device isn't ready.
	if (m_rContext.Begin())
	{
		//------------------
		// Prepare rendering

		// Bind the render target we want to draw in
		m_rContext.Bind(m_oRenderTargetState);

		// Bind camera
		m_rContext.Bind(m_oCameraState);

		//-------
		// Render

		// Render the scene, ie : characters
		RenderScene();

		// Render dummy light object
		RenderDummyLighs();

		//--------------
		// Ends rendering

		// Finally queue the swap command
		m_rContext.Queue(m_oSwapCommand);

		// End of the rendering context
		m_rContext.End();
	}
#endif

}

void HelloWndApp::SetupRenderTarget()
{
	// Lock the data but don't discard previous values
	RenderTargetState::ReadWriteLocker locker(pRenderTargetState_, false);

	// Fetch the render target state data 
	RenderTargetStateData* pData = locker.GetData();

	// Sets color information
	pData->SetColor(Float4(0, 127, 127, 0));
}

#if !defined(FAT_OS_ANDROID)
void HelloWndApp::Run()
{
	SetupRenderTarget();

	Bool loop = true;
	while (loop)
	{
		Bool gotMsg;
		MSG	msg;

		if (GetMainWnd()->IsActive())
		{
			// The PeekMessage function dispatches incoming sent messages, checks the thread message queue 
			// for a posted message, and retrieves the message (if any exist). 
			gotMsg = (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE | PM_NOYIELD) != FALSE);
		}
		else
		{
			// Unlike GetMessage, the PeekMessage function does not wait for a message to be posted before 
			// returning. So GetMessage is used when the application does not have the focus. 
			gotMsg = (GetMessage(&msg, NULL, 0, 0) >= 0);
		}

		// Handle messages
		if (gotMsg)
		{
			// Detect a Quit message to stop the loop
			if (msg.message == WM_QUIT)
			{
				// Exit the while loop
				loop = false;
			}

			// Translate and dispatch messages
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Do the per frame update
			Update();
		}
	}

	// Do post loop code
//	ExitLoop();
}
#endif
