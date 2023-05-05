#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "managerApplications.h"
#include "managerArchives.h"
#include "managerAudio.h"
#include "managerGUI.h"
#include "managerInputDevices.h"
#include "managerRenderTextures.h"
#include "managerShaders.h"
#include "managerSprites.h"
#include "managerTextFonts.h"
#include "managerTextures.h"

namespace Nexus
{
	// Holds pointers to each of the various managers
	class Managers : public Singleton<Managers>
	{
	public:
		// Obtains pointers to each of the managers
		Managers();

		ManagerApplications* applications;
		ManagerArchives* archives;
		ManagerAudio* audio;
		ManagerGUI* gui;
		ManagerInputDevices* input;
		ManagerRenderTextures* renderTextures;
		ManagerShaders* shaders;
		ManagerSprites* sprites;
		ManagerTextFonts* textFonts;
		ManagerTextures* textures;
	};

	
}