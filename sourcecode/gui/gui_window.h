#pragma once
#include "../precompiled_header.h"
#include "../math/vector2.h"
#include "gui_button.h"

namespace Nexus
{
	// A GUI object, a window.
	// A window contains all other GUI objects. I almost called this class a GUIContainer because of this.
	class GUIWindow
	{
		friend class ManagerGUI;
	public:
		// Sets default settings for the window
		GUIWindow();

		// Sets whether a window is enabled or not
		void setWindowEnabled(bool bEnabledIn);

		// Sets the dimensions of the window.
		void setWindowDimensions(int iWidthIn, int iHeightIn);

		// Sets the position of the window
		void setWindowPosition(const Vector2& vNewPosition);

		// Sets the text to be rendered over the titlebar
		void setWindowTitlebarText(const std::string strTitlebarTextIn);

		// Returns window position
		Vector2 getWindowPosition(void) { return vPosition; }

		// Returns window dimensions
		Vector2 getWindowDimensions(void) { return vDimensions; }

		// Adds a new button, ready for customization
		GUIButton* addButton(const std::string& name);

		// Returns a pointer to the named object.
		// Throws an exception if the object couldn't be found
		GUIButton* getButton(const std::string& name);

		// Returns true if the named object exists, else false.
		bool getExistsButton(const std::string& name);

		// Removes a button.
		// Throws an exception if the named object doesn't exist.
		void removeButton(const std::string& name);

	private:
		bool bEnabled;					// Whether this window is enabled or not
		bool bWindowIsJustAContainer;	// Is false, then the window background, edges, corners and text is rendered, else the window acts as just a container for it's other GUI objects.
		Vector2 vDimensions;			// Width and height of the object
		Vector2 vPosition;				// Position of this object relative to it's parent window.
		std::string strTitlebarText;	// Text rendered on top of the object. The titlebar text.
		bool bBeingMoved;				// Whether the window is being moved or not

		

		std::map<std::string, GUIButton*>	mapGUIButtons;	// Each named button.
	};

}
