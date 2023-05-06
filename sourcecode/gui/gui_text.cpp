#include "precompiled_header.h"
#include "gui_text.h"
#include "../managers/managers.h"
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
		Managers* pMan = Managers::getPointer();
		GUITheme* pTheme = pMan->gui->getCurrentTheme();
		Texture* pTextureWindow = pMan->textures->get2DTexture(pTheme->strTexturenameWindow);
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
		Shader* pShader = pMan->shaders->getShader("default");
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
		Managers* pMan = Managers::getPointer();
		GUITheme* pTheme = pMan->gui->getCurrentTheme();
		TextFont* pTextFont = pMan->textFonts->getTextFont(pTheme->strFontnameText);
		
		// Bind the render texture as render target
		pRenderTexture->bindFramebuffer(true, Vector4(0.0f, 0.0f, 0.0f, 0.0f));

		// Split string into words seperated by space
		std::istringstream ss(strText);							// Copy the text into istringstream
		std::string strWord;									// Holds each word
		std::vector<std::string> strLinesToBeRendered;			// Holds each line of text to be rendered
		std::string strLine;									// Single line of text
		int iWidthSpace = (int)pTextFont->getTextWidth(" ");	// Compute width of space character
		// Get each word, one at a time and store in strWord
		while (getline(ss, strWord, ' '))
		{
			// If current line and word fits
			if (pTextFont->getTextWidth(strWord) + pTextFont->getTextWidth(strLine) < vDimensions.x)
			{
				strLine.append(strWord);
				strLine.append(" ");
			}
			else	// Word and current line is too long
			{
				strLinesToBeRendered.push_back(strLine);	// Store current line
				strLine.clear();
				strLine.append(strWord);	// Add word to next line
				strLine.append(" ");
			}
		}
		// Check to see if everything fit on a single line
		if (0==strLinesToBeRendered.size())
		{
			if (strLine.length())
				strLinesToBeRendered.push_back(strLine);	// Store current line
		}

		// Render each line of text
		int iYpos = 0;
		for (int i=0; i<strLinesToBeRendered.size(); ++i)
		{
			pTextFont->print(strLinesToBeRendered[i], 0, iYpos, pRenderTexture->getWidth(), pRenderTexture->getHeight(), pTheme->textColour);
			iYpos += pTheme->textLinesHeight;
		}

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

	void GUIText::setDimensions(float fNewWidth, float fNewHeight)
	{
		Vector2 vNewDimensions(fNewWidth, fNewHeight);
		setDimensions(vNewDimensions);
	}

	void GUIText::setText(const std::string& text)
	{
		if (strText != text)
			bRenderTextureNeedsUpdating = true;
		strText = text;
	}
}