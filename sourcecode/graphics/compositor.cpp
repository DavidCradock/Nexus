#pragma once
#include "precompiled_header.h"
#include "compositor.h"
#include "../managers/managers.h"

namespace Nexus
{
	Compositor::Compositor()
	{
		RenderDevice* pRD = RenderDevice::getPointer();
		Managers* pMan = Managers::getPointer();
		pMan->renderTextures->add("compositor", pRD->getWindowWidth(), pRD->getWindowHeight(), "default");
		vertexBufferQuad.addQuad(Vector2(0, 0), Vector2((float)pRD->getWindowWidth(), (float)pRD->getWindowHeight()));
		vertexBufferQuad.upload();
		
	}

	void Compositor::render(void)
	{
		// Check to see if the window's dimensions have changed and if so, rebuilt the render texture and vertex buffer
		RenderDevice* pRD = RenderDevice::getPointer();
		Managers* pMan = Managers::getPointer();
		RenderTexture* pRenderTexture = pMan->renderTextures->get("compositor");
		bool bResizeNeeded = false;
		if (pRenderTexture->getWidth() != (int)pRD->getWindowWidth())
			bResizeNeeded = true;
		if (pRenderTexture->getHeight() != (int)pRD->getWindowHeight())
			bResizeNeeded = true;
		if (bResizeNeeded)
		{
			vertexBufferQuad.reset();
			vertexBufferQuad.addQuad(Vector2(0, 0), Vector2((float)pRD->getWindowWidth(), (float)pRD->getWindowHeight()));
			vertexBufferQuad.upload();
			pMan->renderTextures->remove("compositor", "default");
			pMan->renderTextures->add("compositor", int(pRD->getWindowWidth()), int(pRD->getWindowHeight()), "default");
			pRenderTexture = pMan->renderTextures->get("compositor");
		}

		// Resizing complete, now render the thing
		Shader* pShader = pMan->shaders->get("default");
		pRenderTexture->bindTexture();
		matrixTransform.setOrthographic();
		pShader->bind();
		pShader->setInt("texture1", pRenderTexture->getTextureID());
	}
	
}