#include "precompiled_header.h"
#include "applicationDevelopment.h"
#include "../managers/managerInputDevices.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"
#include "../graphics/renderDevice.h"
#include "../managers/managerGUI.h"
#include "../managers/managerSprites.h"
#include "../graphics/textFont.h"
#include "../Nexus.h"

namespace Nexus
{
	ApplicationDevelopment::ApplicationDevelopment(std::string& applicationName)
	{
		addAppToManager(applicationName);
	}

	void ApplicationDevelopment::initOnce(void)
	{
		// GUI
		ManagerGUI* pManGUI = ManagerGUI::getPointer();
		GUIWindow *pWindow = pManGUI->addWindow("Test Window2");
		pWindow->setWindowPosition(Vector2(640, 0));
		pWindow->setWindowDimensions(640, 480);

		GUIButton *pButton = pWindow->addButton("Exit button");
		pButton->setText("Exit");
		
		GUIText *pText = pWindow->addText("Text1");
		pText->setText("Hello world! This is lots of text to test the word wrapping code of TextFont with a call to some method or something lol.");
		pText->setPosition(Vector2(0, 100));
		pText->setDimensions(Vector2(640, 100));

		
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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		timing.update();

		// Exit application if exit button clicked on
		if (true == ManagerGUI::getPointer()->getWindow("Test Window2")->getButton("Exit button")->getClicked())
			return false;

		return true;
	}

}