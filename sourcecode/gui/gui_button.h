#pragma once
#include "../precompiled_header.h"
#include "../math/vector2.h"

namespace Nexus
{
	struct TexCoords
	{
		Vector2 vTCTL;
		Vector2 vTCTR;
		Vector2 vTCBL;
		Vector2 vTCBR;
	};

	// A GUI object, a button.
	// A button is a GUI object which may be clicked upon by the user
	class GUIButton
	{
		friend class GUIWindow;
	public:
		// Sets default settings for the button
		GUIButton();

		// Called from GUIWindow to update this object
		void update(GUIWindow* pWindow);

		// Called from GUIWindow to render this object
		void render(GUIWindow *pWindow);

		// Sets the text to be rendered over the object
		void setText(const std::string& text) {strText = text;}
	private:
		Vector2 vDimensions;	// Width and height of the object
		Vector2 vPosition;		// Position of this object relative to it's parent window.
		std::string strText;	// Text rendered on top of the object.
		bool bMouseOver;		// Whether the mourse cursor is over or not

		TexCoords vTexCoordsC;	// Texture coordinates computed in constructor
		TexCoords vTexCoordsTL;	// Texture coordinates computed in constructor
		TexCoords vTexCoordsTR;	// Texture coordinates computed in constructor
		TexCoords vTexCoordsBL;	// Texture coordinates computed in constructor
		TexCoords vTexCoordsBR;	// Texture coordinates computed in constructor
		TexCoords vTexCoordsT;	// Texture coordinates computed in constructor
		TexCoords vTexCoordsB;	// Texture coordinates computed in constructor
		TexCoords vTexCoordsL;	// Texture coordinates computed in constructor
		TexCoords vTexCoordsR;	// Texture coordinates computed in constructor
	};

}
