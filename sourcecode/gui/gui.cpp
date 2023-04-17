#include "precompiled_header.h"
#include "gui.h"
#include "../core/log.h"
#include "../graphics/texture.h"
#include "../graphics/renderDevice.h"

namespace Nexus
{
	GUIManager::GUIManager()
	{
		// Create default theme
		GUITheme* pTheme = createTheme("default");
		strCurrentTheme = "default";
		TextureManager::getPointer()->add2DTexture(pTheme->strTexturenameWindow, pTheme->strTexturenameWindow, "default", false);

		pShader = ShaderManager::getPointer()->getShader("gui", "default");
	}

	void GUIManager::render(void)
	{
		// If there are no windows, do nothing
		if (!mapGUIWindows.size())
			return;

		TextureManager* pTM = TextureManager::getPointer();

		// Get currently set theme
		GUITheme* pTheme = getTheme(strCurrentTheme);
		
		// Windows backgrounds
		Texture* pTexture = pTM->get2DTexture(pTheme->strTexturenameWindow, "default");
		Vector2 vWindowTextureDims((float)pTexture->getWidth(), (float)pTexture->getHeight());
		Vector2 vWndTexDimsDiv3 = vWindowTextureDims;
		vWndTexDimsDiv3.multiply(0.3333333f);

		// Prepare rendering
		RenderDevice* pRD = RenderDevice::getPointer();
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

			vertexBuffer.upload();
			vertexBuffer.draw();

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
			Log::getPointer()->addException(err);
		}

		// If we get here, we have got to create, then add the resource
		GUITheme* pNewRes = new GUITheme();
		mapGUIThemes[name] = pNewRes;

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
			Log::getPointer()->addException(err);
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
			Log::getPointer()->addException(err);
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
			Log::getPointer()->addException(err);
		}

		// If we get here, we have got to create, then add the resource
		GUIWindow* pNewRes = new GUIWindow();
		mapGUIWindows[name] = pNewRes;

		// Find the object to return a pointer to it
		itr = mapGUIWindows.find(name);
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
			Log::getPointer()->addException(err);
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
			Log::getPointer()->addException(err);
		}

		// Destroy the resource
		delete itr->second;
		mapGUIWindows.erase(itr);
	}

}