#include "precompiled_header.h"
#include "applicationDevelopment.h"
#include "../input/inputManager.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../graphics/renderDevice.h"
#include "../graphics/sprites/spriteManager.h"
#include "../graphics/textFont.h"
#include "../gui/gui.h"

namespace Nexus
{
	ApplicationDevelopment::ApplicationDevelopment(std::string& applicationName)
	{
		_addAppToManager(applicationName);
	}

	void ApplicationDevelopment::initOnce(void)
	{
		timing.setStatFPSSrate(5.0f);

		// Build "built-in" fonts
		TextFontManager* pTFM = TextFontManager::getPointer();
		pTFM->buildFontFiles("fonts/PublicSans", "Public Sans", 20, true, false, false, false, false);

		// GUI
		GUIManager* pGUI = GUIManager::getPointer();
		GUIWindow *pWindow = pGUI->createWindow("Test Window1");
		pWindow->setWindowPosition(Vector2(320, 240));
		pWindow->setWindowDimensions(640, 480);

		pWindow = pGUI->createWindow("Test Window2");
//		pWindow->setWindowPosition(Vector2(650, 240));
//		pWindow->setWindowDimensions(640, 480);

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
		if (InputManager::getPointer()->key.pressed(KC_ESCAPE))
			return false;

		static bool vsync = true;
		if (InputManager::getPointer()->key.once(KC_F1))
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