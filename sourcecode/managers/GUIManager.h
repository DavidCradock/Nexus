#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "../gui/gui_button.h"
#include "../gui/gui_window.h"
#include "../core/timing.h"

namespace Nexus
{
	// Use this to deal with everything related to the graphical user interface.
	// Create text, buttons, windows and all sorts of fun interactive things.
	class GUIManager : public Singleton<GUIManager>
	{
	public:
		// Sets defaults and creates the default theme
		GUIManager();

		// Updates the GUI.
		void update(void);

		// Renders the GUI.
		void render(void);

		// Creates a new theme, ready for customization
		GUITheme* addTheme(const std::string& name);

		// Returns a pointer to the named object.
		// Throws an exception if the object couldn't be found
		GUITheme* getTheme(const std::string& name);

		// Returns true if the named object exists, else false.
		bool getExistsTheme(const std::string& name);

		// Removes a theme.
		// Throws an exception if the named object doesn't exist.
		void removeTheme(const std::string& name);

		// Creates a new window, ready for customization
		GUIWindow* addWindow(const std::string& name);

		// Returns a pointer to the named object.
		// Throws an exception if the object couldn't be found
		GUIWindow* getWindow(const std::string& name);

		// Returns true if the named object exists, else false.
		bool getExistsWindow(const std::string& name);

		// Removes a window.
		// Throws an exception if the named object doesn't exist.
		void removeWindow(const std::string& name);

		// Moves window to front zorder
		void moveWindowToFront(const std::string& name);

		// Sets the current theme
		void setCurrentTheme(const std::string& name);

		// Returns the name of the currently set theme
		std::string getCurrentThemeName(void);

		// Returns currently set theme
		GUITheme* getCurrentTheme(void);

		// Loads all themes, adding texure
		void loadAllThemes(void);

		// Returns true is the mouse is over a gui window that is enabled
		bool getMouseOverGUIWindow(void) { return bMouseIsOverGUI; }

		// Returns whether a window is being moved or not
		bool getWindowBeingMoved(void) { return bWindowBeingMoved; }

		// Sets whether a window is being moved or not
		void setWindowBeingMoved(bool bMoved = true) {bWindowBeingMoved = bMoved;}
	private:
		std::map<std::string, GUITheme*>	mapGUIThemes;	// Each named theme.
		std::map<std::string, GUIWindow*>	mapGUIWindows;	// Each named window.
		bool bMouseIsOverGUI;								// Is the mouse cursor over a GUI window that's enabled
		std::string strCurrentTheme;						// Name of the currently set theme
		std::vector<std::string> vecStringWindowZorder;		// Holds names of windows in Z order
		Timing timing;										// Used by the Frame Statistics window
		bool bWindowBeingMoved;								// Prevents multiple windows from being moved if their titlebars are roughly in same location when clicked upon.
		float fTimeToAddValueToFPSLineGraph;				// Used by the Frame Statistics window
	};

}