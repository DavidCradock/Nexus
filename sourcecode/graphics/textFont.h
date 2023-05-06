#pragma once
#include "../precompiled_header.h"
#include "colour.h"
#include "../math/vector2.h"
#include "vertexBuffer.h"

namespace Nexus
{
	// A font rendering object
	// Used by the various GUI objects
	class TextFont
	{
		friend class TextFontManager;
	public:
		// Called when added to resource manager
		TextFont();

		~TextFont();

		// Prints the given text as a single line
		void print(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, const Colouruc& colour = Colouruc(255, 255, 255, 255));

		// Prints the given text as a single line
		void print(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, const Colourf& colour = Colourf(1, 1, 1, 1));

		// Prints the given text as a single line, centered.
		void printCentered(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, const Colouruc& colour = Colouruc(255, 255, 255, 255));

		// Prints the given text as a single line, centered.
		void printCentered(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, const Colourf& colour = Colourf(1, 1, 1, 1));

		// Get the width, in pixels, of the parsed text, if it were to be rendered.
		float getTextWidth(const std::string& strText);

		// Get the maximum height in pixels, of the font
		float getTextHeight(void) { return fontTypes.fMaxCharHeight; }

		
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
		struct CharDesc
		{
			float fABCa;		// The distance to add to the current position before drawing the character glyph.
			float fABCb;		// The width of the drawn portion of the character glyph. 
			float fABCc;		// The distance to add to the current position to provide white space to the right of the character glyph.
			Vector2 vTexMin;	// Bottom left texture coordinates
			Vector2 vTexMax;	// Top right texture coordinates
		};
		// Gets loaded in when calling load()
		struct FontTypes
		{
			CharDesc charDesc[256];		// Each characters dims etc
			std::string strTextureName;	// Name of the texture filename this font "retrieves" the RGBA data from.
			float fMaxCharHeight;			// The tallest character's height

		};
		FontTypes fontTypes;
		VertexBuffer vertexBuffer;		// Used during rendering
		unsigned int refCount;			// Used by the manager 
	};
}