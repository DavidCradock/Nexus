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
		GUIWindow *pWindow = pManGUI->addWindow("Test Window1");
		pWindow->setWindowPosition(Vector2(320, 240));
		pWindow->setWindowDimensions(640, 480);

		GUIButton *pButton = pWindow->addButton("Button1");
		pButton->setText("Button1");
		
		GUIText *pText = pWindow->addText("Text1");
		pText->setText("Hello world!");

		pWindow = pManGUI->addWindow("Frame Statistics");
		pWindow->setWindowPosition(Vector2(320+640+50, 240));
		pWindow->setWindowDimensions(640, 480);
		pText = pWindow->addText("FPSCurrent");
		pText->setPosition(Vector2(0,0));
		pText = pWindow->addText("FPSSmoothed");
		pText->setPosition(Vector2(0, 20));

		// Sprite manager
		ManagerSprites* pManSprites = ManagerSprites::getPointer();
		SpriteDescription *pSpriteDesc = pManSprites->addDescription("spriteTest");
		pSpriteDesc->addFrame("sprites/sprite_test.png");
		SpriteLayer *pSpriteLayer = pManSprites->addLayer("layer0");
		SpriteEntity *pSpriteEntity = pSpriteLayer->addEntity("entity0", "spriteTest", Vector2(500, 500), 0.0f, 1.0f);
		
		
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

		// Set GUIWindow "Frame Statistics" text
		GUIWindow* pWindow = ManagerGUI::getPointer()->getWindow("Frame Statistics");
		GUIText* pText = pWindow->getText("FPSCurrent");
		std::string strTextFPS("Framerate Current: ");
		strTextFPS.append(std::to_string((int)timing.getStatFPS()));
		pText->setText(strTextFPS);
		pText = pWindow->getText("FPSSmoothed");
		strTextFPS = "Framerate Smoothed: ";
		strTextFPS.append(std::to_string((int)timing.getStatFPSS()));
		pText->setText(strTextFPS);

		// Rotate sprite
		ManagerSprites* pSpriteMan = ManagerSprites::getPointer();
		float fSpriteRotDeg = pSpriteMan->getLayer("layer0")->getEntity("entity0", "spriteTest")->getRotationDeg();
		fSpriteRotDeg += (float)timing.getSecPast() * 90.0f;
		pSpriteMan->getLayer("layer0")->getEntity("entity0", "spriteTest")->setRotationDeg(fSpriteRotDeg);
		return true;
	}

}