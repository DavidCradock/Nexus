#pragma once
#include "../precompiled_header.h"
#include "../math/vector2.h"

namespace Nexus
{

	// A GUI object, a button.
	// A button is a GUI object which may be clicked upon by the user
	class GUIButton
	{
	public:
		// Sets default settings for the window
		GUIButton();

		Vector2 vDimensions;	// Width and height of the object
		Vector2 vPosition;		// Position of this object relative to it's parent window.
		std::string strText;	// Text rendered on top of the object.
	};

}
