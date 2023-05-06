#include "precompiled_header.h"
#include "applicationDevelopment.h"
#include "../managers/managers.h"
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
		Managers* pMan = Managers::getPointer();
		GUIWindow *pWindow = pMan->gui->addWindow("Test Window2");
		pWindow->setWindowPosition(Vector2(640 + 110, 0));
		pWindow->setWindowDimensions(640, 480);

		GUIButton *pButton = pWindow->addButton("Exit button");
		pButton->setText("Exit");
		pButton->setPosition(0, 10);

		pButton = pWindow->addButton("VSync");
		pButton->setText("VSync");
		pButton->setPosition(0, 60);

		GUIText *pText = pWindow->addText("Text1");
		pText->setText("Hello world! This is lots of text to test the word wrapping code of TextFont with a call to some method or something lol. Umm, let's type some more texxt so that the text is rendered onto a few lines along the horizontal, no wait, I meant vertical.");
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

		Managers* pMan = Managers::getPointer();

		// Shutdown if escape key pressed
		if (pMan->input->key.pressed(KC_ESCAPE))
			return false;

		static bool vsync = true;
		if (pMan->input->key.once(KC_F1))
		{
			vsync = !vsync;
			RenderDevice::getPointer()->setVsync(vsync);
		}
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		timing.update();

		// Exit application if exit button clicked on
		if (true == pMan->gui->getWindow("Test Window2")->getButton("Exit button")->getClicked())
			return false;

		// Toggle vsync
		if (true == pMan->gui->getWindow("Test Window2")->getButton("VSync")->getClicked())
		{
			RenderDevice* pRD = RenderDevice::getPointer();
			pRD->setVsync(!pRD->getVsync());
		}
		return true;
	}

}