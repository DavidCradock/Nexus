#include "precompiled_header.h"
#include "gui_theme.h"

namespace Nexus
{

	GUITheme::GUITheme()
	{
		strTexturenameWindow = "gui/gui_window.png";
		strFontnameWindowTitlebar = "fonts/publicsans_20";

		strTexturenameButton[0] = "gui/gui_button_up.png";
		strTexturenameButton[1] = "gui/gui_button_over.png";
		strTexturenameButton[2] = "gui/gui_button_down.png";
	}
}