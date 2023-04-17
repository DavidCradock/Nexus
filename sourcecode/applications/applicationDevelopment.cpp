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
		// Add some shaders
		ShaderManager* pSM = ShaderManager::getPointer();
		pSM->addShader("simple", "shaders/simple.vs", "shaders/simple.fs");
		pSM->loadGroup("default");

		vertexBuffer.addQuad(Vector2(0, 0), Vector2(512, 512));

		vertexBuffer.upload();

		timing.setStatFPSSrate(5.0f);
		// Load textures
		TextureManager* pTM = TextureManager::getPointer();
		pTM->add2DTexture("logo", "textures/logo.png");
		pTM->loadGroup("default");

		sprite.setDimensions(Vector2(512, 512));
		sprite.setTexture("logo");

		// Sprite manager
		SpriteManager* pSpriteManager = SpriteManager::getPointer();
		SpriteDescription* pSpriteDesc = pSpriteManager->addDescription("spriteTest");
		pSpriteDesc->addFrame("sprites/sprite_test.png");
		SpriteLayer* pSpriteLayer = pSpriteManager->addLayer("layer0");
		SpriteEntity *pSpriteEntity = pSpriteLayer->addEntity("sprite_entity0", "spriteTest", Vector2(10, 10), 0, 1.0f);
		
		pSpriteDesc = pSpriteManager->addDescription("spriteTest2");
		pSpriteDesc->addFrame("sprites/sprite_test2.png");
		pSpriteEntity = pSpriteLayer->addEntity("sprite_entity1", "spriteTest2", Vector2(10, 110), 0, 0.5f);

		pTM->loadGroup("sprites");

		// Text rendering
		// First build and save the files required by TextFont
		/* No point wasting CPU cycles as files built
		textFont.buildFontFiles("fonts/arial", "arial", 12, true, false, false, false, false);
		textFont.buildFontFiles("fonts/arial", "arial", 24, true, false, false, false, false);
		textFont.buildFontFiles("fonts/arial", "arial", 36, true, false, false, false, false);
		textFont.buildFontFiles("fonts/arial", "arial", 48, true, false, false, false, false);
		textFont.buildFontFiles("fonts/arial", "arial", 96, true, false, false, false, false);
		textFont.buildFontFiles("fonts/impact", "impact", 12, true, false, false, false, false);
		textFont.buildFontFiles("fonts/impact", "impact", 24, true, false, false, false, false);
		textFont.buildFontFiles("fonts/impact", "impact", 36, true, false, false, false, false);
		textFont.buildFontFiles("fonts/impact", "impact", 48, true, false, false, false, false);
		textFont.buildFontFiles("fonts/impact", "impact", 96, true, false, false, false, false);
		/**/
		//textFont.buildFontFiles("fonts/arial", "arial", 48, true, false, false, false, false);

		// Build "built-in" fonts
		textFont.buildFontFiles("fonts/PublicSans", "Public Sans", 40, true, false, false, false, false);

		// Now load a font in
		textFont.load("fonts/PublicSans_40");
		pTM->loadGroup("fonts");

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
		glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		/*
		Texture* pTexture = TextureManager::getPointer()->get2DTexture("logo");
		pTexture->bind();
		
		Shader *pShader = ShaderManager::getPointer()->getShader("simple");
		//pShader->setInt("texture1", pTexture->getID());
		
		timing.update();

		Matrix matrixOrtho;
		matrixOrtho.setOrthographic();
		pShader->setMat4("transform", matrixOrtho);
		pShader->use();
		vertexBuffer.draw();
		*/
		sprite.render(Vector2(512, 512));

		timing.update();
		float fSec = (float)timing.getSecPast();
		// Move sprite
		SpriteEntity* pSpriteEntity = SpriteManager::getPointer()->getLayer("layer0")->getEntity("sprite_entity0", "spriteTest");
		Vector2 vSpritePos = pSpriteEntity->getPosition();
		vSpritePos.x += 10;// fSec * 100.0f;
		if (vSpritePos.x > RenderDevice::getPointer()->getWindowWidth())
			vSpritePos.x = -pSpriteEntity->getSpriteDescDims().x;
		pSpriteEntity->setPosition(vSpritePos);
		pSpriteEntity->setScale(0.5f);

		// Move logo to centre of screen
		pSpriteEntity = SpriteManager::getPointer()->getLayer("layer0")->getEntity("sprite_entity1", "spriteTest2");
		vSpritePos.x = (float)RenderDevice::getPointer()->getWindowWidth() / 2;
		vSpritePos.x -= pSpriteEntity->getRenderDims().x / 2;
		vSpritePos.y = (float)RenderDevice::getPointer()->getWindowHeight() / 2;
		vSpritePos.y -= pSpriteEntity->getRenderDims().y / 2;
		pSpriteEntity->setPosition(vSpritePos);

		SpriteManager::getPointer()->_render();

		std::string text("FPS: ");
		text.append(std::to_string(int(timing.getStatFPSS())));
		float fYpos = 5;
		textFont.print(text, 5, (int)fYpos);																fYpos += textFont.getTextHeight();
		textFont.print("Hello World!", 5, (int)fYpos);														fYpos += textFont.getTextHeight();
		textFont.print("Here is some text to render so I can look at the default fonts.", 5, (int)fYpos);	fYpos += textFont.getTextHeight();
		return true;
	}

}