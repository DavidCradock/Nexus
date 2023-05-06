#include "precompiled_header.h"
#include "managers.h"

namespace Nexus
{
	Managers::Managers()
	{
		applications = ApplicationManager::getPointer();
		archives = ArchiveManager::getPointer();
		audio = AudioManager::getPointer();
		gui = GUIManager::getPointer();
		input = InputDeviceManager::getPointer();
		renderTextures = RenderTextureManager::getPointer();
		shaders = ShaderManager::getPointer();
		sprites = SpriteManager::getPointer();
		textFonts = TextFontManager::getPointer();
		textures = TextureManager::getPointer();
	}
}