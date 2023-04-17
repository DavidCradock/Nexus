#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "gui_button.h"
#include "gui_theme.h"
#include "gui_window.h"
#include "../graphics/vertexBuffer.h"
#include "../graphics/shader.h"

namespace Nexus
{

	// Use this to deal with everything related to the graphical user interface.
	// Create text, buttons, windows and all sorts of fun interactive things.
	class GUIManager : public Singleton<GUIManager>
	{
	public:
		// Sets defaults and creates the default theme
		GUIManager();

		// Updates and renders the GUI.
		// Called from main
		void render(void);

		// Creates a new theme, ready for customization
		GUITheme* createTheme(const std::string& name);

		// Returns a pointer to the named object.
		// Throws an exception if the object couldn't be found
		GUITheme* getTheme(const std::string& name);

		// Returns true if the named object exists, else false.
		bool getExistsTheme(const std::string& name);

		// Removes a theme.
		// Throws an exception if the named object doesn't exist.
		void removeTheme(const std::string& name);

		// Creates a new window, ready for customization
		GUIWindow* createWindow(const std::string& name);

		// Returns a pointer to the named object.
		// Throws an exception if the object couldn't be found
		GUIWindow* getWindow(const std::string& name);

		// Returns true if the named object exists, else false.
		bool getExistsWindow(const std::string& name);

		// Removes a window.
		// Throws an exception if the named object doesn't exist.
		void removeWindow(const std::string& name);

		// Sets the current theme
		void setCurrentTheme(const std::string& name);

		

		// Returns true is the mouse is over a gui window that is enabled
		bool getMouseOverGUIWindow(void) { return bMouseIsOverGUI; }
	private:
		std::map<std::string, GUITheme*>	mapGUIThemes;	// Each named theme.
		std::map<std::string, GUIWindow*>	mapGUIWindows;	// Each named window.
		VertexBuffer vertexBuffer;							// Vertex buffer used for rendering
		Shader* pShader;									// Shader used for rendering.
		bool bMouseIsOverGUI;								// Is the mouse cursor over a GUI window that's enabled
		std::string strCurrentTheme;						// Name of the currently set theme
	};

}