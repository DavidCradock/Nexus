#include "precompiled_header.h"
#include "managers.h"

namespace Nexus
{
	void Managers::init(void)
	{
		applications = ApplicationManager::getPointer();
		archives = ArchiveManager::getPointer();
		audio = AudioManager::getPointer();
		geometry = GeometryManager::getPointer();
		gui = GUIManager::getPointer();
		input = InputDeviceManager::getPointer();
		renderTextures = RenderTextureManager::getPointer();
		shaders = ShaderManager::getPointer();
		sprites = SpriteManager::getPointer();
		textFonts = TextFontManager::getPointer();
		textures = TextureManager::getPointer();

		// Default groups for managers which have groups
		geometry->addNewGroup("default");
		shaders->addNewGroup("default");
		textures->addNewGroup("default");
		textures->addNewGroup("fonts");
		textures->addNewGroup("gui");
		renderTextures->addNewGroup("default");

		// GUI themes
		gui->addTheme("default");
		gui->setCurrentTheme("default");

		// "Core" resources
		shaders->add("default", "shaders/default.vs", "shaders/default.fs", "default");
		shaders->add("vert_texcoord_normal", "shaders/vert_texcoord_normal.vs", "shaders/vert_texcoord_normal.fs", "default");
		textures->add("white_32x32", "textures/white_32x32.png", "default");
		textures->add("checker", "textures/checker.png", "default");

		// Input initialisation. Needs the window to be created otherwise all hell breaks loose :)
		input->init(RenderDevice::getPointer()->getWindowHandle());


	}
}