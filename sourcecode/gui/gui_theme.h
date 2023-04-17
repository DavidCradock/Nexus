#pragma once
#include "../precompiled_header.h"

namespace Nexus
{

	// A theme for the GUI manager to use
	// All texture images' dimensions should be divisable by 3, with each image divided into a 3x3 grid of equal proportions.
	// For example, the image dimensions for the GUIWindow object could be 96x96 where each cell has the dimensions of 32x32.
	// Each of the 9 cells correspond to top left corner, top right corner, bottom right corner, bottom left corner, the four edges and the centre.
	// The cells of the four edges are stretched
	class GUITheme
	{
	public:
		// Sets default settings for the theme using default texture names etc.
		GUITheme();

		std::string strTexturenameWindow;			// Name of the texture containing image data used to render a window.
		std::string strFontnameWindow;				// Name of the font used to render text for a window.
		std::string strTexturenameButton[3];		// Name of the texture containing image data used to render a button in it's up, over and down states.
		
	};

}
