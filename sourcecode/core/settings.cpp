#include "precompiled_header.h"
#include "settings.h"

namespace Nexus
{
	Settings::Settings()
	{
		screenmode = 2;
		screenWidth = 1920;
		screenHeight = 1080;
		vsyncEnabled = true;
	}

	bool Settings::load(std::string filename)
	{
		std::ifstream file;
		file.open(filename);
		if (file.fail())
			return false;

		std::string buffer;
		getline(file, buffer);	// Get "Settings file for Nexus"
		file >> buffer;		file >> buffer;	screenmode = std::stoi(buffer);
		file >> buffer;		file >> buffer; screenWidth = std::stoi(buffer);
		file >> buffer;		file >> buffer; screenHeight = std::stoi(buffer);
		file >> buffer;		file >> buffer; vsyncEnabled = std::stoi(buffer);
		if (file.fail())
		{
			file.close();
			return false;
		}
		file.close();
		return true;
	}

	bool Settings::save(std::string filename)
	{
		std::ofstream file;
		file.open(filename);
		if (!file.is_open())
			return false;
		file << "Settings file for Nexus" << std::endl;
		file << "screenmode: " << screenmode << std::endl;
		file << "screenWidth: " << screenWidth << std::endl;
		file << "screenHeight: " << screenHeight << std::endl;
		file << "vsyncEnabled: " << vsyncEnabled << std::endl;
		file.close();
		return true;
	}



}