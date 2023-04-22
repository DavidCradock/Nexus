#pragma once
#include "../precompiled_header.h"

namespace Nexus
{
	// Loading and saving of all Mojo related settings such as display mode, dims, etc
	class Settings
	{
	public:

		// Upon construction, sets default values
		Settings();

		// Loads all settings from given file.
		bool load(std::string filename = "settings.cfg");

		// Saves all settings to given file.
		bool save(std::string filename = "settings.cfg");

		int screenmode;				// 0=True fullscreen. 1=borderless window fullscreen. 2=decorated window
		unsigned int screenWidth;
		unsigned int screenHeight;
		int vsyncEnabled;
	};

}