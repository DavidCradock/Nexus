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
		pWindow->setWindowEnabled(false);

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

		// Geometry
		pMan->geometry->convertObj("geometry/blender_default_cube.obj");	// Load, convert and save
		pMan->geometry->add("geometry/blender_default_cube.geom");
		pMan->geometry->convertObj("geometry/blender_monkey.obj");	// Load, convert and save
		pMan->geometry->add("geometry/blender_monkey.geom");
		pMan->geometry->loadGroup("default");

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

		// Test rendering of geometry
		Matrix matView;
		Matrix matProjection;
		matView.setView(Vector3(0, 0, -10),	// Eye pos
						Vector3(0, 0, 0),	// Target pos
						Vector3(0, 1, 0));	// Up vector
		matProjection.setProjection(
			85.0f,	// FOV degrees
			pRD->getWindowWidth(),
			pRD->getWindowHeight(),
			0.1f,	// Near
			1000);	// Far
		Matrix matViewProjection = matProjection;
//		matViewProjection *= matView;
		Shader* pShader = pMan->shaders->get("vert_texcoord_normal", "default");

		// Temp, use glm
		static float fRot = 0.0f;
		fRot += timing.getSecPast();
		glm::mat4x4 matrix(1.0f);
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(sinf(fRot)*5.0f, sinf(fRot)*5.0f, cosf(fRot) * 5.0f),	//eye
			glm::vec3(0.0f, 1.5f, 0.0f),//target
			glm::vec3(0.0f, 1.0f, 0.0f));//up
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), float((float)pRD->getWindowWidth() / (float)pRD->getWindowHeight()), 0.1f, 1000.0f);
		glm::mat4 model(1.0f);
		matrix = projection * view;
		pShader->use();
		pShader->setMat4("transform", matrix);
		Texture* pTexture = pMan->textures->get("checker");
		pTexture->bind();
		pShader->setInt("texture1", pTexture->getID());
//		Geometry* pGeometry = pMan->geometry->get("geometry/blender_default_cube.geom");
		Geometry* pGeometry = pMan->geometry->get("geometry/blender_monkey.geom");
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		pGeometry->draw(false);

		return true;
	}

}