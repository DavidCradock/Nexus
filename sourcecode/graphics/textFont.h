#pragma once
#include "../precompiled_header.h"
#include "colour.h"
#include "../math/vector2.h"
#include "vertexBuffer.h"

namespace Nexus
{
	// A font rendering object
	class TextFont
	{
		friend class ManagerTextFonts;
	public:
		// Called when added to resource manager
		TextFont();

		~TextFont();

		// Prints the given text to the current RT.
		// This is single line, left aligned.
		// It's a quick and dirty "I want some text without any hassle" method
		// strText is the text to be rendered.
		// iPosX is the left position of the clip rect
		// iPosY is the top position of the clip rect
		// colour is the RGB and alpha colour to use to render the text.
		void print(const std::string& strText, int iPosX, int iPosY, const CColouruc& colour = CColouruc(255, 255, 255, 255));

		// Get the width, in pixels, of the parsed text, if it were to be rendered.
		float getTextWidth(const std::string& strText);

		// Get the maximum height in pixels, of the font
		float getTextHeight(void) { return fontTypes.mfMaxCharHeight; }

		
	private:
		// loads the text font from the file pair.
		// Only adds the texture name to the texture manager, so don't forget to call the manager's load method, for the "fonts" group
		// If already added, nothing happens
		void load(const std::string& strFontFilePairName);

		// Removes the texture used by the text font from texture manager
		// If already unloaded, nothing happens
		void unload(void);

		bool bLoaded;	// Whether the resource is loaded or not

		// Each character description (width, height, offset etc
		struct SCharDesc
		{
			float fABCa;		// The distance to add to the current position before drawing the character glyph.
			float fABCb;		// The width of the drawn portion of the character glyph. 
			float fABCc;		// The distance to add to the current position to provide white space to the right of the character glyph.
			Vector2 vTexMin;	// Bottom left texture coordinates
			Vector2 vTexMax;	// Top right texture coordinates
		};
		// Gets loaded in when calling load()
		struct SFontTypes
		{
			SCharDesc mCharDesc[256];		// Each characters dims etc
			std::string mstrTextureName;	// Name of the texture filename this font "retrieves" the RGBA data from.
			float mfMaxCharHeight;			// The tallest character's height

		};
		SFontTypes fontTypes;
		VertexBuffer vertexBuffer;		// Used during rendering
		unsigned int refCount;			// Used by the manager 
	};
}