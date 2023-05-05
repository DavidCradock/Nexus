#include "precompiled_header.h"
#include "managers.h"

namespace Nexus
{
	Managers::Managers()
	{
		applications = ManagerApplications::getPointer();
		archives = ManagerArchives::getPointer();
		audio = ManagerAudio::getPointer();
		gui = ManagerGUI::getPointer();
		input = ManagerInputDevices::getPointer();
		renderTextures = ManagerRenderTextures::getPointer();
		shaders = ManagerShaders::getPointer();
		sprites = ManagerSprites::getPointer();
		textFonts = ManagerTextFonts::getPointer();
		textures = ManagerTextures::getPointer();
	}
}