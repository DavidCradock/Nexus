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

		strTexturenameButton[0] = "gui/gui_button_up.png";
		strTexturenameButton[1] = "gui/gui_button_over.png";
		strTexturenameButton[2] = "gui/gui_button_down.png";
	}
}