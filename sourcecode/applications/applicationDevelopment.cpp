#include "precompiled_header.h"
#include "applicationDevelopment.h"
#include "../managers/managerInputDevices.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../graphics/renderDevice.h"
#include "../managers/managerGUI.h"
#include "../managers/managerSprites.h"
#include "../graphics/textFont.h"


namespace Nexus
{
	ApplicationDevelopment::ApplicationDevelopment(std::string& applicationName)
	{
		_addAppToManager(applicationName);
	}

	void ApplicationDevelopment::initOnce(void)
	{
		timing.setStatFPSSrate(5.0f);

		// GUI
		ManagerGUI* pGUI = ManagerGUI::getPointer();
		GUIWindow *pWindow = pGUI->createWindow("Test Window1");
		pWindow->setWindowPosition(Vector2(320, 240));
		pWindow->setWindowDimensions(640, 480);

		pWindow = pGUI->createWindow("Test Window2");
		pWindow->setWindowPosition(Vector2(320+640+50, 240));
		pWindow->setWindowDimensions(640, 480);

	}

	void ApplicationDevelopment::onStart(void)
	{

	}

	void ApplicationDevelopment::onStop(void)
	{

	}

	bool ApplicationDevelopment::onUpdate(void)
	{
		RenderDevice* pRD = RenderDevice::getPointer();

		// Shutdown if escape key pressed
		if (ManagerInputDevices::getPointer()->key.pressed(KC_ESCAPE))
			return false;

		static bool vsync = true;
		if (ManagerInputDevices::getPointer()->key.once(KC_F1))
		{
			vsync = !vsync;
			RenderDevice::getPointer()->setVsync(vsync);
		}
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		timing.update();

		return true;
	}

}