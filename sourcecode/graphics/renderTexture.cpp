#include "precompiled_header.h"
#include "renderTexture.h"
#include "../core/log.h"
#include "renderDevice.h"

namespace Nexus
{

	RenderTexture::RenderTexture(int iWidth, int iHeight)
	{
		refCount = 0;
		colourTextureID = 0;
		renderBufferID = 0;
		width = iWidth;
		height = iHeight;

		// Create and bind framebuffer object
		glGenFramebuffers(1, &frameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

		// Create and setup the colour attachment texture
		glGenTextures(1, &colourTextureID);
		glBindTexture(GL_TEXTURE_2D, colourTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTextureID, 0);

		glGenRenderbuffers(1, &renderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, iWidth, iHeight); // Use a single renderbuffer object for both a depth and stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID); // Attach it
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error("RenderTarget() failed. Framebuffer is not complete.");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	RenderTexture::~RenderTexture()
	{
		glDeleteRenderbuffers(1, &renderBufferID);
		glDeleteFramebuffers(1, &frameBufferObject);
		glDeleteTextures(1, &colourTextureID);
	}

	void RenderTexture::bindTexture(void)
	{
		glBindTexture(GL_TEXTURE_2D, colourTextureID);
	}

	void RenderTexture::bindFramebuffer(bool bClear, Vector4 clearColour)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
		glViewport(0, 0, width, height);
		glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
	}

	void RenderTexture::unbind(void)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		RenderDevice* pRD = RenderDevice::getPointer();
		glViewport(0, 0, pRD->getWindowWidth(), pRD->getWindowHeight());
	}

	unsigned int RenderTexture::getTextureID(void)
	{
		return colourTextureID;
	}

	int RenderTexture::getWidth(void)
	{
		return width;
	}

	int RenderTexture::getHeight(void)
	{
		return height;
	}
}