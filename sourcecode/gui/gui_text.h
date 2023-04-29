#pragma once
#include "../precompiled_header.h"
#include "../math/vector2.h"
#include "../graphics/renderTexture.h"

namespace Nexus
{
	// A GUI object, text.
	class GUIText
	{
		friend class GUIWindow;
	public:
		// Sets default settings for the text
		GUIText();

		// Called from GUIWindow to render this object
		void render(GUIWindow *pWindow);

		// Sets position within it's parent window
		void setPosition(const Vector2& vNewPosition) { vPosition = vNewPosition; }

		// Sets dimensions of the object for word wrapping
		void setDimensions(const Vector2& vNewDimensions);

		// Sets the text to be rendered over the object
		void setText(const std::string& text);

	private:
		// Checks to see if the texture needs updating, or needs creating and does so.
		// Called from render()
		void renderToTexture(void);

		Vector2 vDimensions;	// Width and height of the object
		Vector2 vPosition;		// Position of this object relative to it's parent window.
		std::string strText;	// Text rendered on top of the object.
		RenderTexture* pRenderTexture;	// The texture that holds the rendered text
		bool bRenderTextureNeedsUpdating;	
	};

}
