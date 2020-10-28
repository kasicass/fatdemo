#include "FatFramework/FatFramework.h"

#if FAT_OS_ANDROID

#include <android_native_app_glue.h>

using namespace Fat;

extern "C" {

///
// HandleCommand()
//
//    Android callback for onAppCmd
//
static void HandleCommand(struct android_app *pApp, int32_t cmd)
{
	switch (cmd)
	{
	case APP_CMD_SAVE_STATE:
		// the OS asked us to save the state of the app
		break;

	case APP_CMD_INIT_WINDOW:
		GApp->Init();
		break;

	case APP_CMD_TERM_WINDOW:
		GApp->Shutdown();
		break;

	case APP_CMD_LOST_FOCUS:
		// if the app lost focus, avoid unnecessary processing (like monitoring the accelerometer)
		break;

	case APP_CMD_GAINED_FOCUS:
		// bring back a certain functionality, like monitoring the accelerometer
		break;
	}
}

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  android_main()
//
//    Main entrypoint for Android application
//
void android_main(struct android_app *pApp)
{
	pApp->onAppCmd = HandleCommand;
	pApp->userData = GApp;

	while (1)
	{
		int ident;
		int events;
		struct android_poll_source *pSource;

		while ((ident = ALooper_pollAll(0, NULL, &events, (void **)&pSource)) >= 0)
		{
			if (pSource != NULL)
			{
				pSource->process(pApp, pSource);
			}

			if (pApp->destroyRequested != 0)
			{
				return;
			}
		}

		// Call app update function
		// GApp->Update();

		// GApp->Draw();
	}
}

}

#endif
