#pragma once
#include "../precompiled_header.h"
#include "../math/vector4.h"

namespace Nexus
{
	// A single render texture
	class RenderTexture
	{
		friend class RenderTextureManager;

	public:
		RenderTexture(int iWidth, int iHeight);

		~RenderTexture();

		// Binds the texture, so it may be used as a normal texture
		void bindTexture(void);

		// Binds the framebuffer object, so all future rendering will be into the framebuffer
		void bindFramebuffer(bool bClear = true, Vector4 clearColour = Vector4(0.0f, 0.0f, 0.0f, 1.0f));

		// Unbinds both the texture and the framebuffer object
		void unbind(void);

		// Returns the OpenGL texture ID
		unsigned int getTextureID(void);

		// Returns width of image used for the texture.
		int getWidth(void);

		// Returns width of image used for the texture.
		int getHeight(void);
	private:
		unsigned int refCount;			// Used by the manager 
		unsigned int colourTextureID;	// OpenGL ID for colour
		unsigned int renderBufferID;	// OpenGL ID for render buffer
		unsigned int frameBufferObject;	
		int width;
		int height;
	};
}