#include "precompiled_header.h"
#include "gui_window.h"
#include "../core/log.h"
#include "../managers/managers.h"
#include "../managers/managedObjects/shader.h"
#include "../graphics/renderDevice.h"

namespace Nexus
{

	GUIWindow::GUIWindow()
	{
		bEnabled = true;
		bWindowIsJustAContainer = false;
		vDimensions.set(320, 240);
		bBeingMoved = false;
		bMouseIsOverWindow = false;

		// Compute texture coordinates for each of the 9 components
		float point3 = 0.3333333f;
		float point6 = 0.6666666f;

		// Centre quad
		vTexCoordsC.vTCBL.x = point3;
		vTexCoordsC.vTCTR.x = point6;
		vTexCoordsC.vTCBR.x = point6;
		vTexCoordsC.vTCTL.x = point3;
		vTexCoordsC.vTCBL.y = point3;
		vTexCoordsC.vTCTR.y = point6;
		vTexCoordsC.vTCBR.y = point3;
		vTexCoordsC.vTCTL.y = point6;

		// Top left corner
		vTexCoordsTL.vTCBL.x = 0.0f;
		vTexCoordsTL.vTCTR.x = point3;
		vTexCoordsTL.vTCBR.x = point3;
		vTexCoordsTL.vTCTL.x = 0.0f;
		vTexCoordsTL.vTCBL.y = point3;
		vTexCoordsTL.vTCTR.y = 0.0f;
		vTexCoordsTL.vTCBR.y = point3;
		vTexCoordsTL.vTCTL.y = 0.0f;

		// Top right corner
		vTexCoordsTR.vTCBL.x = point6;
		vTexCoordsTR.vTCTR.x = 1.0f;
		vTexCoordsTR.vTCBR.x = 1.0f;
		vTexCoordsTR.vTCTL.x = point6;
		vTexCoordsTR.vTCBL.y = point3;
		vTexCoordsTR.vTCTR.y = 0.0f;
		vTexCoordsTR.vTCBR.y = point3;
		vTexCoordsTR.vTCTL.y = 0.0f;

		// Bottom left corner
		vTexCoordsBL.vTCBL.x = 0.0f;
		vTexCoordsBL.vTCTR.x = point3;
		vTexCoordsBL.vTCBR.x = point3;
		vTexCoordsBL.vTCTL.x = 0.0f;
		vTexCoordsBL.vTCBL.y = 1.0f;
		vTexCoordsBL.vTCTR.y = point6;
		vTexCoordsBL.vTCBR.y = 1.0f;
		vTexCoordsBL.vTCTL.y = point6;

		// Bottom right corner
		vTexCoordsBR.vTCBL.x = point6;
		vTexCoordsBR.vTCTR.x = 1.0f;
		vTexCoordsBR.vTCBR.x = 1.0f;
		vTexCoordsBR.vTCTL.x = point6;
		vTexCoordsBR.vTCBL.y = 1.0f;
		vTexCoordsBR.vTCTR.y = point6;
		vTexCoordsBR.vTCBR.y = 1.0f;
		vTexCoordsBR.vTCTL.y = point6;

		// Top edge
		vTexCoordsT.vTCBL.x = point3;
		vTexCoordsT.vTCTR.x = point6;
		vTexCoordsT.vTCBR.x = point6;
		vTexCoordsT.vTCTL.x = point3;
		vTexCoordsT.vTCBL.y = point3;
		vTexCoordsT.vTCTR.y = 0;
		vTexCoordsT.vTCBR.y = point3;
		vTexCoordsT.vTCTL.y = 0;

		// Bottom edge
		vTexCoordsB.vTCBL.x = point3;
		vTexCoordsB.vTCTR.x = point6;
		vTexCoordsB.vTCBR.x = point6;
		vTexCoordsB.vTCTL.x = point3;
		vTexCoordsB.vTCBL.y = 1;
		vTexCoordsB.vTCTR.y = point6;
		vTexCoordsB.vTCBR.y = 1;
		vTexCoordsB.vTCTL.y = point6;

		// Left edge
		vTexCoordsL.vTCBL.x = 0;
		vTexCoordsL.vTCTR.x = point3;
		vTexCoordsL.vTCBR.x = point3;
		vTexCoordsL.vTCTL.x = 0;
		vTexCoordsL.vTCBL.y = point3;
		vTexCoordsL.vTCTR.y = point6;
		vTexCoordsL.vTCBR.y = point3;
		vTexCoordsL.vTCTL.y = point6;

		// Right edge
		vTexCoordsR.vTCBL.x = point6;
		vTexCoordsR.vTCTR.x = 1.0f;
		vTexCoordsR.vTCBR.x = 1.0f;
		vTexCoordsR.vTCTL.x = point6;
		vTexCoordsR.vTCBL.y = point3;
		vTexCoordsR.vTCTR.y = point6;
		vTexCoordsR.vTCBR.y = point3;
		vTexCoordsR.vTCTL.y = point6;
	}

	bool GUIWindow::update(const std::string& strWindowName)
	{
		RenderDevice* pRenderDevice = RenderDevice::getPointer();
		Managers* pMan = Managers::getPointer();
		
		GUITheme* pTheme = pMan->gui->getCurrentTheme();
		Texture* pTexture = pMan->textures->get(pTheme->strTexturenameWindow, "default");
		Vector2 vWindowTextureDims((float)pTexture->getWidth(), (float)pTexture->getHeight());
		Vector2 vWndTexDimsDiv3 = vWindowTextureDims;
		vWndTexDimsDiv3.multiply(0.3333333f);
		// Mouse info
		Vector2 vMousePosDelta = pMan->input->mouse.getMouseDeltaGUI();
		Vector2 vMousePosCurrent = pMan->input->mouse.getCursorPos();
		// Application window information
		Vector2 vApplicationWindowDims((float)pRenderDevice->getWindowWidth(), (float)pRenderDevice->getWindowHeight());

		bMouseIsOverWindow = false;

		// If window is disabled
		if (false == bEnabled)
			return bMouseIsOverWindow;

		// Get window position and dimensions
		Vector2 vWindowPos = getWindowPosition();
		Vector2 vWindowDims = getWindowDimensions();

		// Is mouse over window?
		if (vMousePosCurrent.x > vWindowPos.x)
		{
			if (vMousePosCurrent.x < vWindowPos.x + vWindowDims.x)
			{
				if (vMousePosCurrent.y > vWindowPos.y)
				{
					if (vMousePosCurrent.y < vWindowPos.y + vWindowDims.y)
					{
						// If we get here, mouse cursor is over the window including edges
						bMouseIsOverWindow = true;

						// Is mouse over titlebar?
						if (vMousePosCurrent.y < vWindowPos.y + vWndTexDimsDiv3.y)
						{
							if (pMan->input->mouse.leftButtonOnce())
							{
								if (!pMan->gui->getWindowBeingMoved())
								{
									pMan->gui->setWindowBeingMoved(true);
									bBeingMoved = true;
									pMan->gui->moveWindowToFront(strWindowName);
								}
							}
						}
					}
				}
			}
		}

		// Stop moving window if mouse not down
		if (!pMan->input->mouse.leftButDown())
		{
			bBeingMoved = false;
		}

		// Move window
		if (bBeingMoved)
		{
			vPosition.x += pMan->input->mouse.getMouseDeltaGUI().x;
			vPosition.y += pMan->input->mouse.getMouseDeltaGUI().y;

			// Limit to screen
			if (vPosition.x < 0)
				vPosition.x = 0;
			if (vPosition.y < 0)
				vPosition.y = 0;
			if (vPosition.x + vDimensions.x + (2.0f * vWndTexDimsDiv3.x) > vApplicationWindowDims.x)
				vPosition.x = vApplicationWindowDims.x - vDimensions.x - (2.0f * vWndTexDimsDiv3.x);
			if (vPosition.y + vDimensions.y + (2.0f * vWndTexDimsDiv3.y) > vApplicationWindowDims.y)
				vPosition.y = vApplicationWindowDims.y - vDimensions.y - (2.0f * vWndTexDimsDiv3.y);
		}

		// Each button in current window
		if (bMouseIsOverWindow)
		{
			std::map<std::string, GUIButton*>::iterator itb = mapGUIButtons.begin();
			while (itb != mapGUIButtons.end())
			{
				itb->second->update(this);
				itb++;
			}
		}
		return bMouseIsOverWindow;
	}

	void GUIWindow::render(void)
	{
		// If window is disabled
		if (false == bEnabled)
			return;
		Managers* pMan = Managers::getPointer();
		RenderDevice* pRenderDevice = RenderDevice::getPointer();

		GUITheme* pTheme = pMan->gui->getCurrentTheme();
		Texture* pTextureWindow = pMan->textures->get(pTheme->strTexturenameWindow, "default");
		Vector2 vTextureWindowDims((float)pTextureWindow->getWidth(), (float)pTextureWindow->getHeight());
		Vector2 vTextureWindowDimsDiv3 = vTextureWindowDims;
		vTextureWindowDimsDiv3.multiply(0.3333333f);
		Shader* pShader = pMan->shaders->get("default");
		Vector2 vFinalPos;
		Vector2 vFinalDims;

		// Prepare rendering of background
		pTextureWindow->bind();
		
		pShader->bind();
		pShader->setInt("texture1", pTextureWindow->getID());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		Matrix matrixOrtho;
		matrixOrtho.setOrthographic();
		Matrix matrixTransform;
		matrixTransform.setIdentity();
		VertexBuffer vertexBuffer;
		vertexBuffer.reset();
		matrixTransform.setTranslation(vPosition.x, vPosition.y, 0.0f);
		pShader->setMat4("transform", matrixOrtho * matrixTransform);

		// Centre quad
		vFinalPos.x = vTextureWindowDimsDiv3.x;
		vFinalPos.y = 0.0f;
		vFinalPos.y += vTextureWindowDimsDiv3.y;
		vFinalDims.x = vDimensions.x;
		vFinalDims.y = vDimensions.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector4(1.0f, 1.0f, 1.0f, 1.0f), vTexCoordsC.vTCBL, vTexCoordsC.vTCBR, vTexCoordsC.vTCTR, vTexCoordsC.vTCTL);

		// Top left corner
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = 0;
		vFinalPos.y = 0;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector4(1.0f, 1.0f, 1.0f, 1.0f), vTexCoordsTL.vTCBL, vTexCoordsTL.vTCBR, vTexCoordsTL.vTCTR, vTexCoordsTL.vTCTL);

		// Top right corner
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = vDimensions.x + vTextureWindowDimsDiv3.x;
		vFinalPos.y = 0;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector4(1.0f, 1.0f, 1.0f, 1.0f), vTexCoordsTR.vTCBL, vTexCoordsTR.vTCBR, vTexCoordsTR.vTCTR, vTexCoordsTR.vTCTL);

		// Bottom left corner
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = 0;
		vFinalPos.y = vDimensions.y + vTextureWindowDimsDiv3.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector4(1.0f, 1.0f, 1.0f, 1.0f), vTexCoordsBL.vTCBL, vTexCoordsBL.vTCBR, vTexCoordsBL.vTCTR, vTexCoordsBL.vTCTL);

		// Bottom right corner
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = vDimensions.x + vTextureWindowDimsDiv3.x;
		vFinalPos.y = vDimensions.y + vTextureWindowDimsDiv3.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector4(1.0f, 1.0f, 1.0f, 1.0f), vTexCoordsBR.vTCBL, vTexCoordsBR.vTCBR, vTexCoordsBR.vTCTR, vTexCoordsBR.vTCTL);

		// Top edge
		vFinalDims.x = vDimensions.x;
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = vTextureWindowDimsDiv3.x;
		vFinalPos.y = 0.0f;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector4(1.0f, 1.0f, 1.0f, 1.0f), vTexCoordsT.vTCBL, vTexCoordsT.vTCBR, vTexCoordsT.vTCTR, vTexCoordsT.vTCTL);

		// Bottom edge
		vFinalDims.x = vDimensions.x;
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = vTextureWindowDimsDiv3.x;
		vFinalPos.y = vDimensions.y + vTextureWindowDimsDiv3.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector4(1.0f, 1.0f, 1.0f, 0.5f), vTexCoordsB.vTCBL, vTexCoordsB.vTCBR, vTexCoordsB.vTCTR, vTexCoordsB.vTCTL);

		// Left edge
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = vDimensions.y;
		vFinalPos.x = 0.0f;
		vFinalPos.y = vTextureWindowDimsDiv3.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector4(1.0f, 1.0f, 1.0f, 0.5f), vTexCoordsL.vTCBL, vTexCoordsL.vTCBR, vTexCoordsL.vTCTR, vTexCoordsL.vTCTL);

		// Right edge
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = vDimensions.y;
		vFinalPos.x = vDimensions.x + vTextureWindowDimsDiv3.x;
		vFinalPos.y = vTextureWindowDimsDiv3.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector4(1.0f, 1.0f, 1.0f, 0.5f), vTexCoordsR.vTCBL, vTexCoordsR.vTCBR, vTexCoordsR.vTCTR, vTexCoordsR.vTCTL);

		// Render the background cells
		vertexBuffer.upload();
		vertexBuffer.draw();

		// Now render the text for the titlebar
		if (strTitlebarText.length())
		{
			TextFont* pTextFont = pMan->textFonts->get(pTheme->strFontnameWindowTitlebar);
			pTextFont->print(strTitlebarText,
				(int)vPosition.x + (int)vTextureWindowDimsDiv3.x + (int)pTheme->vWindowTitlebarTextOffset.x,
				(int)vPosition.y + (int)pTheme->vWindowTitlebarTextOffset.y,
				pRenderDevice->getWindowWidth(), pRenderDevice->getWindowHeight(),
				pTheme->windowTitlebarTextColour);
		}

		// Render each button
		std::map<std::string, GUIButton*>::iterator itb = mapGUIButtons.begin();
		while (itb != mapGUIButtons.end())
		{
			itb->second->render(this);
			itb++;
		}

		// Render each text object
		std::map<std::string, GUIText*>::iterator itt = mapGUIText.begin();
		while (itt != mapGUIText.end())
		{
			itt->second->render(this);
			itt++;
		}

		// Render each linegraph object
		std::map<std::string, GUILineGraph*>::iterator itlg = mapGUILineGraph.begin();
		while (itlg != mapGUILineGraph.end())
		{
			itlg->second->render(this);
			itlg++;
		}
	}

	void GUIWindow::setWindowEnabled(bool bEnabledIn)
	{
		bEnabled = bEnabledIn;
	}

	void GUIWindow::setWindowDimensions(int iWidthIn, int iHeightIn)
	{
		vDimensions.x = (float)iWidthIn;
		vDimensions.y = (float)iHeightIn;
	}

	void GUIWindow::setWindowPosition(const Vector2& vNewPosition)
	{
		vPosition = vNewPosition;
	}

	void GUIWindow::setWindowTitlebarText(const std::string strTitlebarTextIn)
	{
		strTitlebarText = strTitlebarTextIn;
	}

	GUIButton* GUIWindow::addButton(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, GUIButton*>::iterator itr = mapGUIButtons.find(name);
		if (mapGUIButtons.end() != itr)
		{
			std::string err("GUIWindow::addButton(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object already exists.");
			Log::getPointer()->exception(err);
		}

		// If we get here, we have got to create, then add the resource
		GUIButton* pNewRes = new GUIButton();
		mapGUIButtons[name] = pNewRes;

		// Find the object to return a pointer to it
		itr = mapGUIButtons.find(name);
		return (GUIButton*)itr->second;
	}

	GUIButton* GUIWindow::getButton(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, GUIButton*>::iterator itr = mapGUIButtons.find(name);
		if (mapGUIButtons.end() == itr)
		{
			std::string err("GUIWindow::getButton(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object doesn't exist.");
			Log::getPointer()->exception(err);
		}
		return (GUIButton*)itr->second;
	}

	bool GUIWindow::getExistsButton(const std::string& name)
	{
		std::map<std::string, GUIButton*>::iterator itr = mapGUIButtons.find(name);
		if (itr == mapGUIButtons.end())
			return false;
		return true;
	}

	void GUIWindow::removeButton(const std::string& name)
	{
		// Resource doesn't exist in the group?
		std::map<std::string, GUIButton*>::iterator itr = mapGUIButtons.find(name);
		if (mapGUIButtons.end() == itr)
		{
			std::string err("GUIWindow::removeButton(\"");
			err.append(name);
			err.append("\") failed because the named object couldn't be found.");
			Log::getPointer()->exception(err);
		}

		// Destroy the resource
		delete itr->second;
		mapGUIButtons.erase(itr);
	}

	GUIText* GUIWindow::addText(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, GUIText*>::iterator itr = mapGUIText.find(name);
		if (mapGUIText.end() != itr)
		{
			std::string err("GUIWindow::addText(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object already exists.");
			Log::getPointer()->exception(err);
		}

		// If we get here, we have got to create, then add the resource
		GUIText* pNewRes = new GUIText();
		mapGUIText[name] = pNewRes;

		// Find the object to return a pointer to it
		itr = mapGUIText.find(name);
		return (GUIText*)itr->second;
	}

	GUIText* GUIWindow::getText(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, GUIText*>::iterator itr = mapGUIText.find(name);
		if (mapGUIText.end() == itr)
		{
			std::string err("GUIWindow::getText(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object doesn't exist.");
			Log::getPointer()->exception(err);
		}
		return (GUIText*)itr->second;
	}

	bool GUIWindow::getExistsText(const std::string& name)
	{
		std::map<std::string, GUIText*>::iterator itr = mapGUIText.find(name);
		if (itr == mapGUIText.end())
			return false;
		return true;
	}

	void GUIWindow::removeText(const std::string& name)
	{
		// Resource doesn't exist in the group?
		std::map<std::string, GUIText*>::iterator itr = mapGUIText.find(name);
		if (mapGUIText.end() == itr)
		{
			std::string err("GUIWindow::removeText(\"");
			err.append(name);
			err.append("\") failed because the named object couldn't be found.");
			Log::getPointer()->exception(err);
		}

		// Destroy the resource
		delete itr->second;
		mapGUIText.erase(itr);
	}

	GUILineGraph* GUIWindow::addLineGraph(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, GUILineGraph*>::iterator itr = mapGUILineGraph.find(name);
		if (mapGUILineGraph.end() != itr)
		{
			std::string err("GUIWindow::addLineGraph(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object already exists.");
			Log::getPointer()->exception(err);
		}

		// If we get here, we have got to create, then add the resource
		GUILineGraph* pNewRes = new GUILineGraph();
		mapGUILineGraph[name] = pNewRes;

		// Find the object to return a pointer to it
		itr = mapGUILineGraph.find(name);
		return (GUILineGraph*)itr->second;
	}

	GUILineGraph* GUIWindow::getLineGraph(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, GUILineGraph*>::iterator itr = mapGUILineGraph.find(name);
		if (mapGUILineGraph.end() == itr)
		{
			std::string err("GUIWindow::getLineGraph(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object doesn't exist.");
			Log::getPointer()->exception(err);
		}
		return (GUILineGraph*)itr->second;
	}

	bool GUIWindow::getExistsLineGraph(const std::string& name)
	{
		std::map<std::string, GUILineGraph*>::iterator itr = mapGUILineGraph.find(name);
		if (itr == mapGUILineGraph.end())
			return false;
		return true;
	}

	void GUIWindow::removeLineGraph(const std::string& name)
	{
		// Resource doesn't exist in the group?
		std::map<std::string, GUILineGraph*>::iterator itr = mapGUILineGraph.find(name);
		if (mapGUILineGraph.end() == itr)
		{
			std::string err("GUIWindow::removeLineGraph(\"");
			err.append(name);
			err.append("\") failed because the named object couldn't be found.");
			Log::getPointer()->exception(err);
		}

		// Destroy the resource
		delete itr->second;
		mapGUILineGraph.erase(itr);
	}
}