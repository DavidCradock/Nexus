#pragma once
#include "../precompiled_header.h"
#include "../graphics/colour.h"
#include "../graphics/textFont.h"
#include "../math/vector2.h"
#include "../graphics/vertexBuffer.h"
#include "../core/singleton.h"

namespace Nexus
{
	class TextFontManager : public Singleton<TextFontManager>
	{
	public:
		// Creates a new object
		// You'll need to call loadAll() afterwards
		TextFont* add(const std::string& name);

		// Returns a pointer to the named object.
		// Throws an exception if the object couldn't be found
		TextFont* get(const std::string& name);

		// Returns true if the named object exists, else false.
		bool getExists(const std::string& name);

		// Removes an object.
		// Throws an exception if the named object doesn't exist.
		void remove(const std::string& name);

		// Loads all added objects if they haven't already
		void loadAll(void);

		// Builds a font and saves it to disk using font files installed on the current OS which can then be used by this classes load() method in the future.
		// The output file names (the font.fnt and font.tga files) are named based upon the strOutputBaseName.
		// For example, if the basename was BASE, the font height 12, then the output files would be BASE_12.fnt and BASE_12.tga
		// If an error occurred, an error message is shown and execution ends.
		// This doesn't alter anything within this object.
		void buildFontFiles(const std::string& strOutputBaseName, const std::string& strFontName = "arial", unsigned int iFontHeight = 12, bool bAntialiased = true, bool bBold = false, bool bItalic = false, bool bUnderlined = false, bool bStrikeout = false);

	private:
		std::map<std::string, TextFont*> mapTextFonts;	// Hash map holding named TextFont object
	};
}