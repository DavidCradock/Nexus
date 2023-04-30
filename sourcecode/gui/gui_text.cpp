#include "precompiled_header.h"
#include "gui_text.h"
#include "../managers/managerGUI.h"
#include "../managers/managerInputDevices.h"
#include "../managers/managerShaders.h"
#include "../managers/managerTextFonts.h"
#include "../managers/managerTextures.h"
#include "../graphics/vertexBuffer.h"
#include "../core/log.h"

namespace Nexus
{
	GUIText::GUIText()
	{
		vDimensions.set(200, 200);
		strText.append("text");
		bRenderTextureNeedsUpdating = true;
		pRenderTexture = 0;
	}

	void GUIText::render(GUIWindow* pWindow)
	{
		ManagerGUI* pManGUI = ManagerGUI::getPointer();
		ManagerTextures* pManTextures = ManagerTextures::getPointer();
		ManagerTextFonts* pManTextFonts = ManagerTextFonts::getPointer();
		GUITheme* pTheme = pManGUI->getCurrentTheme();
		Texture* pTextureWindow = pManTextures->get2DTexture(pTheme->strTexturenameWindow);
		Vector2 vTextureWindowDims((float)pTextureWindow->getWidth(), (float)pTextureWindow->getHeight());
		Vector2 vTextureWindowDimsDiv3 = vTextureWindowDims;
		vTextureWindowDimsDiv3.multiply(0.3333333f);
		Vector2 vWindowPos = pWindow->getWindowPosition();

		Vector2 vOffset;
		if (false == pWindow->getWindowIsContainer())
		{
			vOffset.x += vTextureWindowDimsDiv3.x;
			vOffset.y += vTextureWindowDimsDiv3.y;
		}
		vOffset.x += vWindowPos.x;
		vOffset.y += vWindowPos.y;

		// Compute top left position of the text
		Vector2 vTextPosition;
		vTextPosition.x = vPosition.x;
		vTextPosition.x += vOffset.x;
		vTextPosition.y = vPosition.y;
		vTextPosition.y += vOffset.y;

		// Checks to see if the texture needs updating, or needs creating and does so
		renderToTexture();

		VertexBuffer vertexBuffer;
		vertexBuffer.addQuad(vTextPosition, vDimensions, Vector4(1,1,1,1));
		vertexBuffer.upload();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		Shader* pShader = ManagerShaders::getPointer()->getShader("gui");
		pShader->use();
		pShader->setInt("texture1", pRenderTexture->getTextureID());
		pRenderTexture->bindTexture();
		Matrix matrixOrtho;
		matrixOrtho.setOrthographic();
		Matrix matrixTransform;
		matrixTransform.setIdentity();
		pShader->setMat4("transform", matrixOrtho * matrixTransform);
		vertexBuffer.draw();
		glDisable(GL_BLEND);

	}

	void GUIText::renderToTexture(void)
	{
		// Create render texture if doesn't exist
		if (0 == pRenderTexture)
		{
			pRenderTexture = new RenderTexture((int)vDimensions.x, (int)vDimensions.y);
			if (!pRenderTexture)
				Log::getPointer()->exception("GUIText::renderToTexture() failed. Memory allocation error.");
		}
		// If doesn't need updating, simply return
//		if (false == bRenderTextureNeedsUpdating)
//			return;

		// If we get here, we need to render the text to the render texture
		ManagerGUI* pManGUI = ManagerGUI::getPointer();
		ManagerTextFonts* pManTextFonts = ManagerTextFonts::getPointer();
		GUITheme* pTheme = pManGUI->getCurrentTheme();
		TextFont* pTextFont = pManTextFonts->getTextFont(pTheme->strFontnameText);
		
		// Bind the render texture as render target
		pRenderTexture->bindFramebuffer(true, Vector4(0.0f, 0.0f, 0.0f, 0.0f));

		// Split string into words seperated by space
		std::istringstream ss(strText);		// Copy the text into istringstream
		std::string strWord;				// Holds each word
		while (getline(ss, strWord, ' '))	// For each word, place into strWord
		{
			// Do stuff
		}
		pTextFont->print(strText, 0, 0, pRenderTexture->getWidth(), pRenderTexture->getHeight(), pTheme->textColour);

		// Restore window framebuffer as render target
		pRenderTexture->unbind();
	}

	void GUIText::setDimensions(const Vector2& vNewDimensions)
	{
		if (vDimensions != vNewDimensions)
		{
			if (0 != pRenderTexture)
			{
				delete pRenderTexture;
				pRenderTexture = 0;
			}
		}
		bRenderTextureNeedsUpdating = true;
		vDimensions = vNewDimensions;
	}

	void GUIText::setText(const std::string& text)
	{
		if (strText != text)
			bRenderTextureNeedsUpdating = true;
		strText = text;
	}
}