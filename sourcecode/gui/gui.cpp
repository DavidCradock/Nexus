#include "precompiled_header.h"
#include "gui.h"
#include "../core/log.h"
#include "../graphics/texture.h"
#include "../graphics/renderDevice.h"
#include "../graphics/textFont.h"

namespace Nexus
{
	GUIManager::GUIManager()
	{
		pShader = ShaderManager::getPointer()->getShader("gui", "default");
	}

	void GUIManager::render(void)
	{
		// If there are no windows, do nothing
		if (!mapGUIWindows.size())
			return;

		if (!strCurrentTheme.length())
			throw std::runtime_error("GUIManager::render() failed as currently set theme is not set.");

		TextureManager* pTM = TextureManager::getPointer();
		TextFontManager* pTFM = TextFontManager::getPointer();

		// Get currently set theme
		GUITheme* pTheme = getTheme(strCurrentTheme);
		
		// Windows backgrounds
		Texture* pTexture = pTM->get2DTexture(pTheme->strTexturenameWindow, "default");
		Vector2 vWindowTextureDims((float)pTexture->getWidth(), (float)pTexture->getHeight());
		Vector2 vWndTexDimsDiv3 = vWindowTextureDims;
		vWndTexDimsDiv3.multiply(0.3333333f);

		RenderDevice* pRD = RenderDevice::getPointer();

		
		// For each window
		std::map<std::string, GUIWindow*>::iterator itr = mapGUIWindows.begin();
		Vector2 vFinalPos;
		Vector2 vFinalDims;
		
		while (itr != mapGUIWindows.end())
		{
			// If window is disabled
			if (!itr->second->bEnabled)
			{
				itr++;
				continue;
			}

			// Prepare rendering of background
			pTexture->bind();
			Shader* pShader = ShaderManager::getPointer()->getShader("gui");
			pShader->use();
			pShader->setInt("texture1", pTexture->getID());
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);

			Matrix matrixOrtho;
			matrixOrtho.setOrthographic();
			Matrix matrixTransform;
			matrixTransform.setIdentity();

			vertexBuffer.reset();

			matrixTransform.setTranslation(itr->second->vPosition.x, itr->second->vPosition.y, 0.0f);
			pShader->setMat4("transform", matrixOrtho * matrixTransform);

			// Centre quad
			vFinalPos.x = vWndTexDimsDiv3.x;
			vFinalPos.y = 0.0f;
			vFinalPos.y += vWndTexDimsDiv3.y;
			vFinalDims.x = itr->second->vDimensions.x;
			vFinalDims.x -= vWndTexDimsDiv3.x * 2.0f;
			vFinalDims.y = itr->second->vDimensions.y;
			vFinalDims.y -= vWndTexDimsDiv3.y * 2.0f;
			vertexBuffer.addQuad(vFinalPos, vFinalDims,	Vector3(1.0f, 1.0f, 1.0f), itr->second->vTexCoordsC.vTCBL, itr->second->vTexCoordsC.vTCBR, itr->second->vTexCoordsC.vTCTR, itr->second->vTexCoordsC.vTCTL);

			// Top left corner
			vFinalDims.x = vWndTexDimsDiv3.x;
			vFinalDims.y = vWndTexDimsDiv3.y;
			vFinalPos.x = 0;
			vFinalPos.y = 0;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), itr->second->vTexCoordsTL.vTCBL, itr->second->vTexCoordsTL.vTCBR, itr->second->vTexCoordsTL.vTCTR, itr->second->vTexCoordsTL.vTCTL);

			// Top right corner
			vFinalDims.x = vWndTexDimsDiv3.x;
			vFinalDims.y = vWndTexDimsDiv3.y;
			vFinalPos.x = itr->second->vDimensions.x - vWndTexDimsDiv3.x;
			vFinalPos.y = 0;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), itr->second->vTexCoordsTR.vTCBL, itr->second->vTexCoordsTR.vTCBR, itr->second->vTexCoordsTR.vTCTR, itr->second->vTexCoordsTR.vTCTL);

			// Bottom left corner
			vFinalDims.x = vWndTexDimsDiv3.x;
			vFinalDims.y = vWndTexDimsDiv3.y;
			vFinalPos.x = 0;
			vFinalPos.y = itr->second->vDimensions.y - vWndTexDimsDiv3.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), itr->second->vTexCoordsBL.vTCBL, itr->second->vTexCoordsBL.vTCBR, itr->second->vTexCoordsBL.vTCTR, itr->second->vTexCoordsBL.vTCTL);

			// Bottom right corner
			vFinalDims.x = vWndTexDimsDiv3.x;
			vFinalDims.y = vWndTexDimsDiv3.y;
			vFinalPos.x = itr->second->vDimensions.x - vWndTexDimsDiv3.x;
			vFinalPos.y = itr->second->vDimensions.y - vWndTexDimsDiv3.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), itr->second->vTexCoordsBR.vTCBL, itr->second->vTexCoordsBR.vTCBR, itr->second->vTexCoordsBR.vTCTR, itr->second->vTexCoordsBR.vTCTL);

			// Top edge
			vFinalDims.x = itr->second->vDimensions.x - (vWndTexDimsDiv3.x * 2.0f);
			vFinalDims.y = vWndTexDimsDiv3.y;
			vFinalPos.x = vWndTexDimsDiv3.x;
			vFinalPos.y = 0.0f;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), itr->second->vTexCoordsT.vTCBL, itr->second->vTexCoordsT.vTCBR, itr->second->vTexCoordsT.vTCTR, itr->second->vTexCoordsT.vTCTL);

			// Bottom edge
			vFinalDims.x = itr->second->vDimensions.x - (vWndTexDimsDiv3.x * 2.0f);
			vFinalDims.y = vWndTexDimsDiv3.y;
			vFinalPos.x = vWndTexDimsDiv3.x;
			vFinalPos.y = itr->second->vDimensions.y - vWndTexDimsDiv3.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), itr->second->vTexCoordsB.vTCBL, itr->second->vTexCoordsB.vTCBR, itr->second->vTexCoordsB.vTCTR, itr->second->vTexCoordsB.vTCTL);

			// Left edge
			vFinalDims.x = vWndTexDimsDiv3.x;
			vFinalDims.y = itr->second->vDimensions.y - (vWndTexDimsDiv3.y * 2.0f);
			vFinalPos.x = 0.0f;
			vFinalPos.y = vWndTexDimsDiv3.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), itr->second->vTexCoordsL.vTCBL, itr->second->vTexCoordsL.vTCBR, itr->second->vTexCoordsL.vTCTR, itr->second->vTexCoordsL.vTCTL);

			// Right edge
			vFinalDims.x = vWndTexDimsDiv3.x;
			vFinalDims.y = itr->second->vDimensions.y - (vWndTexDimsDiv3.y * 2.0f);
			vFinalPos.x = itr->second->vDimensions.x - vWndTexDimsDiv3.x;
			vFinalPos.y = vWndTexDimsDiv3.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), itr->second->vTexCoordsR.vTCBL, itr->second->vTexCoordsR.vTCBR, itr->second->vTexCoordsR.vTCTR, itr->second->vTexCoordsR.vTCTL);

			// Render the background cells
			vertexBuffer.upload();
			vertexBuffer.draw();

			// Now render the text for the titlebar
			if (itr->second->strTitlebarText.length())
			{
				TextFont* pTextFont = pTFM->get(pTheme->strFontnameWindowTitlebar);
				pTextFont->print(itr->second->strTitlebarText, (int)vWndTexDimsDiv3.x + 5, (int)vWndTexDimsDiv3.y);
			}
			itr++;
		}
	}

	GUITheme* GUIManager::createTheme(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.find(name);
		if (mapGUIThemes.end() != itr)
		{
			std::string err("GUIManager::createTheme(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object already exists.");
			throw std::runtime_error(err);
		}

		// If we get here, we have got to create, then add the resource
		GUITheme* pNewRes = new GUITheme();
		mapGUIThemes[name] = pNewRes;

		// If this is the first theme, set it as the currently used theme
		if (1 == mapGUIThemes.size())
		{
			setCurrentTheme(name);
		}

		// Find the object to return a pointer to it
		itr = mapGUIThemes.find(name);
		return (GUITheme*)itr->second;
	}

	GUITheme* GUIManager::getTheme(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.find(name);
		if (mapGUIThemes.end() == itr)
		{
			std::string err("GUIManager::getTheme(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object doesn't exist.");
			throw std::runtime_error(err);
		}
		return (GUITheme*)itr->second;
	}

	bool GUIManager::getExistsTheme(const std::string& name)
	{
		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.find(name);
		if (itr == mapGUIThemes.end())
			return false;
		return true;
	}

	void GUIManager::removeTheme(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.find(name);
		if (mapGUIThemes.end() == itr)
		{
			std::string err("GUIManager::removeTheme(\"");
			err.append(name);
			err.append("\") failed because the named object couldn't be found.");
			throw std::runtime_error(err);
		}

		// Destroy the resource
		delete itr->second;
		mapGUIThemes.erase(itr);
	}

	GUIWindow* GUIManager::createWindow(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, GUIWindow*>::iterator itr = mapGUIWindows.find(name);
		if (mapGUIWindows.end() != itr)
		{
			std::string err("GUIManager::createWindow(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object already exists.");
			throw std::runtime_error(err);
		}

		// If we get here, we have got to create, then add the resource
		GUIWindow* pNewRes = new GUIWindow();
		mapGUIWindows[name] = pNewRes;

		// Find the object to return a pointer to it
		itr = mapGUIWindows.find(name);
		itr->second->setWindowTitlebarText(name);	// Also set a default name used when rendering the window's titlebar text
		return (GUIWindow*)itr->second;
	}

	GUIWindow* GUIManager::getWindow(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, GUIWindow*>::iterator itr = mapGUIWindows.find(name);
		if (mapGUIWindows.end() == itr)
		{
			std::string err("GUIManager::getWindow(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object doesn't exist.");
			throw std::runtime_error(err);
		}
		return (GUIWindow*)itr->second;
	}

	bool GUIManager::getExistsWindow(const std::string& name)
	{
		std::map<std::string, GUIWindow*>::iterator itr = mapGUIWindows.find(name);
		if (itr == mapGUIWindows.end())
			return false;
		return true;
	}

	void GUIManager::removeWindow(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, GUIWindow*>::iterator itr = mapGUIWindows.find(name);
		if (mapGUIWindows.end() == itr)
		{
			std::string err("GUIManager::removeWindow(\"");
			err.append(name);
			err.append("\") failed because the named object couldn't be found.");
			throw std::runtime_error(err);
		}

		// Destroy the resource
		delete itr->second;
		mapGUIWindows.erase(itr);
	}

	void GUIManager::setCurrentTheme(const std::string& name)
	{
		strCurrentTheme = name;
	}

	void GUIManager::loadAllThemes(void)
	{
		TextureManager* pTM = TextureManager::getPointer();
		TextFontManager* pTFM = TextFontManager::getPointer();

		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.begin();
		while (itr != mapGUIThemes.end())
		{
			if (!itr->second->bLoaded)
			{
				itr->second->bLoaded = true;
				pTM->add2DTexture(itr->second->strTexturenameWindow, itr->second->strTexturenameWindow, "default", true, TextureFiltering::linear);
				pTM->add2DTexture(itr->second->strTexturenameButton[0], itr->second->strTexturenameButton[0], "default", true, TextureFiltering::linear);
				pTM->add2DTexture(itr->second->strTexturenameButton[1], itr->second->strTexturenameButton[1], "default", true, TextureFiltering::linear);
				pTM->add2DTexture(itr->second->strTexturenameButton[2], itr->second->strTexturenameButton[2], "default", true, TextureFiltering::linear);
				pTFM->create(itr->second->strFontnameWindowTitlebar);
			}
			itr++;
		}
		pTFM->loadAll();
	}
}