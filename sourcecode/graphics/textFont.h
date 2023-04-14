#pragma once
#include "../precompiled_header.h"
#include "colour.h"
#include "../math/vector2.h"
#include "vertexBuffer.h"

namespace Nexus
{
	// A font rendering object
	// All of these methods in here are slow for rendering lots of text.
	// If the text doesn't change, use the GUI to render text... it caches text to a RT, greatly increasing speed.
	class TextFont
	{
	public:
		// Called when added to resource manager
		// strFontFilename is the name of the font filename
		TextFont();

		~TextFont();

		// Returns the maximum height of a character within this font
		//int getMaxHeight(void);

		// Prints the given text to the current RT.
		// This is single line, left aligned.
		// It's a quick and dirty "I want some text without any hassle" method
		// strText is the text to be rendered.
		// iPosX is the left position of the clip rect
		// iPosY is the top position of the clip rect
		// colour is the RGB and alpha colour to use to render the text.
		void print(const std::string& strText, int iPosX, int iPosY, const CColouruc& colour = CColouruc(255, 255, 255, 255));

		// Prints the given text to the current RT.
		// This is the more robust version of print
		// strText is the text to be rendered.
		// iPosX is the left position of the clip rect
		// iPosY is the top position of the clip rect
		// iWidth is the width of the clip rect
		// iHeight is the height of the clip rect
		// dwFormat can be any COMBINATION of the following...
		// DT_BOTTOM Justifies the text to the bottom of the rectangle. This value must be combined with DT_SINGLELINE.
		// DT_CALCRECT Determines the width and height of the rectangle. If there are multiple lines of text, ID3DXFont::DrawText uses the width of the rectangle pointed to by the pRect parameter and extends the base of the rectangle to bound the last line of text. If there is only one line of text, ID3DXFont::DrawText modifies the right side of the rectangle so that it bounds the last character in the line. In either case, ID3DXFont::DrawText returns the height of the formatted text but does not draw the text.
		// DT_CENTER Centers text horizontally in the rectangle.
		// DT_EXPANDTABS Expands tab characters. The default number of characters per tab is eight.
		// DT_LEFT Aligns text to the left.
		// DT_NOCLIP Draws without clipping. ID3DXFont::DrawText is somewhat faster when DT_NOCLIP is used.
		// DT_RIGHT Aligns text to the right.
		// DT_RTLREADING Displays text in right-to-left reading order for bidirectional text when a Hebrew or Arabic font is selected. The default reading order for all text is left-to-right.
		// DT_SINGLELINE Displays text on a single line only. Carriage returns and line feeds do not break the line.
		// DT_TOP Top-justifies text.
		// DT_VCENTER Centers text vertically (single line only).
		// DT_WORDBREAK Breaks words. Lines are automatically broken between words if a word would extend past the edge of the rectangle specified by the pRect parameter. A carriage return/line feed sequence also breaks the line.
		// iTextNumChars Is the number of character within strText to render. If set to -1, all characters in strText will be rendered
		// colour is the RGB and alpha colour to use to render the text.
		// if bCalcHeight is true, then, instead of rendering the text, the height of the text is returned
		//int printex(const CText& strText, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwFormat = DT_LEFT, int iTextNumChars = -1, const CColouruc& colour = CColouruc(255, 255,255, 255), bool bCalculateHeight = false);


		// Computes width and height of rendered text
		//CVector2 getPrintcodesDims(const CText& strText);

		// Prints the parsed strText inside a rectangular area defined by the parsed parameters.
		// This automatically word wraps text and also supports text codes.
		// The text codes should be included within the strText to be rendered
		// Here's an example of their usage...
		// printcodes("Line0[!nl]Line1", 0, 0, 100, 100);	// Prints "Line0", then goes down a line and prints "Line1"
		// "[!b]bold";										// Prints "bold" where the word "bold" is rendered with increased weight
		// "[!i]italic"										// Prints "italic" where the word "italic" is rendered italic
		// "[!ul]Underlined text!"							// Underlined text
		// "[!n]normal text!"								// Normal text
		// "[!colff000099]Red![!col00ff00ff]Blue!"			// RGBA colour of text in hexidecimal form. If the colour is missing a value, or typed wrong, it is ignored.
		// if bComputeHeight = true, the text isn't rendered, but the height coverage if rendered is returned
		//int printcodes(const CText& strText, int iPosX, int iPosY, int iWidth, int iHeight, const CColouruc& defaultColour, bool bComputeHeight = false);

		// Get the width, in pixels, of the parsed text, if it were to be rendered.
		float getTextWidth(const std::string& strText);

		// Get the maximum height in pixels, of the font
		float getTextHeight(void) { return fontTypes.mfMaxCharHeight; }

		// Builds a font and saves it to disk using font files installed on the current OS which can then be used by this classes load() method in the future.
		// The output file names (the font.fnt and font.tga files) are named based upon the strOutputBaseName.
		// For example, if the basename was BASE, the font height 12, then the output files would be BASE_12.fnt and BASE_12_Normal.tga, BASE_12_Bold.tga, BASE_12_Italic.tga, BASE_12_Underlined.tga and BASE_12_Strikeout.tga
		// If an error occurred, an error message is shown and execution ends.
		// This doesn't alter anything within this object.
		void buildFontFiles(const std::string& strOutputBaseName, const std::string& strFontName = "arial", unsigned int iFontHeight = 12, bool bAntialiased = true, bool bBold = false, bool bItalic = false, bool bUnderlined = false, bool bStrikeout = false);

		void load(const std::string& strFontFilePairName);
	
		void unload(void);
	private:
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
	};
}