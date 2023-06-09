#pragma once
#include "../precompiled_header.h"
#include "../managers/managedObjects/textFont.h"

namespace Nexus
{

	// A theme for the GUI manager to use
	// All texture images' dimensions should be divisable by 3, with each image divided into a 3x3 grid of equal proportions.
	// For example, the image dimensions for the GUIWindow object could be 96x96 where each cell has the dimensions of 32x32.
	// Each of the 9 cells correspond to top left corner, top right corner, bottom right corner, bottom left corner, the four edges and the centre.
	// The cells of the four edges are stretched
	class GUITheme
	{
		friend class GUIManager;
		friend class GUIWindow;
		friend class GUIButton;
		friend class GUIText;
		friend class GUILineGraph;
	public:
		// Sets default settings for the theme using default texture names etc.
		GUITheme();

	private:
		bool bLoaded;	// Whether the object has been loaded. Used by the GUI manager.

		// GUIWindow related
		std::string strTexturenameWindow;			// Name of the texture containing image data used to render a window.
		std::string strFontnameWindowTitlebar;		// Name of the font used to render text for a window.
		Vector2 vWindowTitlebarTextOffset;			// Offset used when rendering the text
		TextFont textFontWindowTitlebar;			// TextFont object used to render the window's titlebar text
		Colourf windowTitlebarTextColour;			// Colour to render the titlebar text with

		// GUIButton related
		std::string strTexturenameButton[3];		// Name of the texture containing image data used to render a button in it's up, over and down states.
		std::string strFontnameButton;				// Name of the font used to render text for a button.
		Vector2 vButtonTextOffset;					// Offset used when rendering the text
		TextFont textFontButton;					// TextFont object used to render button text
		Colourf buttonTextColour[3];				// Up, over and down colours for button text

		// GUIText related
		std::string strFontnameText;				// Name of the font used to render the text.
		TextFont textFontText;						// TextFont object used to render text
		Colourf textColour;							// Colour to render the text with
		int textLinesHeight;						// Amount of pixels to move down when rendering multiple lines of text

		// Line graph related
		std::string strFontnameLinegraph;			// Name of the font used to render the text.
		TextFont textFontLinegraph;					// TextFont object used to render the text.
		Colourf linegraphTextColour;				// Colour of the text
		Colourf linegraphBackgroundColour;			// Background colour of the linegraph
	};

}
