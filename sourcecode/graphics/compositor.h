#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "vertexBuffer.h"

namespace Nexus
{
	// Compositor which stuff is rendered to a render texture and then that texture is
	// bound as a texture and rendered over the window's backbuffer with a 2D quad.
	// This allows fancy post processing such as blur etc.
	class Compositor
	{
	public:
		Compositor();

		// Sets the render texture of the compositor as the render target
		void bindAsRenderTarget(void);

		// Renders the compositor's render texture to a fullscreen quad
		void render(void);
	private:
		VertexBuffer vertexBufferQuad;
		Matrix matrixTransform;
		 
	};
}