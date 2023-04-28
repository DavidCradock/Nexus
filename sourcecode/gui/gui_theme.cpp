#include "precompiled_header.h"
#include "gui_theme.h"

namespace Nexus
{

	GUITheme::GUITheme()
	{
		bLoaded = false;
		strTexturenameWindow = "gui/gui_window.png";
		strFontnameWindowTitlebar = "gui/publicsans_20";
		vWindowTitlebarTextOffset.set(-40, 15);
		windowTitlebarTextColour.set(1, 1, 1, 1);

		strTexturenameButton[0] = "gui/gui_button_up.png";
		strTexturenameButton[1] = "gui/gui_button_over.png";
		strTexturenameButton[2] = "gui/gui_button_down.png";
		strFontnameButton = "gui/publicsans_20";
		vButtonTextOffset.set(0, -2);
		buttonTextColour[0].set(0.8f, 0.8f, 0.8f, 1);
		buttonTextColour[1].set(0.9f, 0.9f, 0.9f, 1);
		buttonTextColour[2].set(1, 1, 1, 1);

		strFontnameText = "gui/publicsans_20";
		textColour.set(0.8f, 0.8f, 0.8f, 1.0f);
	}
}