#include "precompiled_header.h"
#include "managerGUI.h"
#include "../core/log.h"
#include "managerShaders.h"
#include "managerTextures.h"
#include "../graphics/renderDevice.h"
#include "managerTextFonts.h"
#include "managerInputDevices.h"

namespace Nexus
{
	ManagerGUI::ManagerGUI()
	{
		pShader = ManagerShaders::getPointer()->getShader("gui", "default");
		bMouseIsOverGUI = false;

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

	void ManagerGUI::update(void)
	{
		// If there are no windows, do nothing
		if (!mapGUIWindows.size())
			return;

		if (!strCurrentTheme.length())
			Log::getPointer()->exception("ManagerGUI::render() failed as currently set theme is not set.");

		// Get currently set theme dims
		GUITheme* pTheme = getTheme(strCurrentTheme);
		ManagerTextures* pManTextures = ManagerTextures::getPointer();
		Texture* pTexture = pManTextures->get2DTexture(pTheme->strTexturenameWindow, "default");
		Vector2 vWindowTextureDims((float)pTexture->getWidth(), (float)pTexture->getHeight());
		Vector2 vWndTexDimsDiv3 = vWindowTextureDims;
		vWndTexDimsDiv3.multiply(0.3333333f);

		// Mouse info
		ManagerInputDevices* pManInputDevices = ManagerInputDevices::getPointer();
		Vector2 vMousePosDelta = pManInputDevices->mouse.getMouseDeltaGUI();
		Vector2 vMousePosCurrent = pManInputDevices->mouse.getCursorPos();
		
		// For each window
		std::map<std::string, GUIWindow*>::iterator itr = mapGUIWindows.begin();
		bMouseIsOverGUI = false;
		while (itr != mapGUIWindows.end())
		{
			// If window is disabled
			if (!itr->second->bEnabled)
			{
				itr++;
				continue;
			}

			// Get window position and dimensions
			Vector2 vWindowPos = itr->second->getWindowPosition();
			Vector2 vWindowDims = itr->second->getWindowDimensions();

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
							bMouseIsOverGUI = true;

							// Is mouse over titlebar?
							if (vMousePosCurrent.y < vWindowPos.y + vWndTexDimsDiv3.y)
							{
								if (pManInputDevices->mouse.leftButtonOnce())
								{
									itr->second->bBeingMoved = true;
									moveWindowToFront(itr->first);
								}
							}

						}
					}
				}
			}

			// Stop moving window if mouse not down
			if (!pManInputDevices->mouse.leftButDown())
			{
				itr->second->bBeingMoved = false;
			}

			// Move window
			if (itr->second->bBeingMoved)
			{
				itr->second->vPosition.x += pManInputDevices->mouse.getMouseDeltaGUI().x;
				itr->second->vPosition.y += pManInputDevices->mouse.getMouseDeltaGUI().y;

				// Limit to screen
				if (itr->second->vPosition.x < 0)
					itr->second->vPosition.x = 0;
				if (itr->second->vPosition.y < 0)
					itr->second->vPosition.y = 0;
				if (itr->second->vPosition.x + itr->second->vDimensions.x > RenderDevice::getPointer()->getWindowWidth())
					itr->second->vPosition.x = RenderDevice::getPointer()->getWindowWidth() - itr->second->vDimensions.x;
				if (itr->second->vPosition.y + itr->second->vDimensions.y > RenderDevice::getPointer()->getWindowHeight())
					itr->second->vPosition.y = RenderDevice::getPointer()->getWindowHeight() - itr->second->vDimensions.y;
			}
			itr++;
		}

	}

	void ManagerGUI::render(void)
	{
		// If there are no windows, do nothing
		if (!mapGUIWindows.size())
			return;

		if (!strCurrentTheme.length())
			Log::getPointer()->exception("ManagerGUI::render() failed as currently set theme is not set.");

		// For each window, using z-ordering with rendering back most first
		for (int iWindow = (int)vecStringWindowZorder.size() - 1; iWindow >= 0; iWindow--)
		{
			std::map<std::string, GUIWindow*>::iterator itw = mapGUIWindows.find(vecStringWindowZorder[iWindow]);
			renderWindow(itw->second);
		}
	}

	void ManagerGUI::renderWindow(GUIWindow* pWindow)
	{
		// If window is disabled
		if (!pWindow->bEnabled)
			return;

		ManagerTextures* pManTextures = ManagerTextures::getPointer();
		ManagerTextFonts* pManTextFonts = ManagerTextFonts::getPointer();
		ManagerInputDevices* pManInput = ManagerInputDevices::getPointer();
		ManagerShaders* pManShaders = ManagerShaders::getPointer();
		RenderDevice* pRenderDevice = RenderDevice::getPointer();
		
		GUITheme* pTheme = getTheme(strCurrentTheme);
		Texture* pTextureWindow = pManTextures->get2DTexture(pTheme->strTexturenameWindow, "default");
		Vector2 vTextureWindowDims((float)pTextureWindow->getWidth(), (float)pTextureWindow->getHeight());
		Vector2 vTextureWindowDimsDiv3 = vTextureWindowDims;
		vTextureWindowDimsDiv3.multiply(0.3333333f);
		Shader* pShader = pManShaders->getShader("gui");
		Vector2 vFinalPos;
		Vector2 vFinalDims;

		
		// If window is just a container
//		if (pWindow->bWindowIsJustAContainer)
//			return;

		// Prepare rendering of background
		pTextureWindow->bind();
		pShader->setInt("texture1", pTextureWindow->getID());
		pShader->use();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		Matrix matrixOrtho;
		matrixOrtho.setOrthographic();
		Matrix matrixTransform;
		matrixTransform.setIdentity();
		vertexBuffer.reset();
		matrixTransform.setTranslation(pWindow->vPosition.x, pWindow->vPosition.y, 0.0f);
		pShader->setMat4("transform", matrixOrtho * matrixTransform);

		// Centre quad
		vFinalPos.x = vTextureWindowDimsDiv3.x;
		vFinalPos.y = 0.0f;
		vFinalPos.y += vTextureWindowDimsDiv3.y;
		vFinalDims.x = pWindow->vDimensions.x;
		vFinalDims.x -= vTextureWindowDimsDiv3.x * 2.0f;
		vFinalDims.y = pWindow->vDimensions.y;
		vFinalDims.y -= vTextureWindowDimsDiv3.y * 2.0f;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsC.vTCBL, vTexCoordsC.vTCBR, vTexCoordsC.vTCTR, vTexCoordsC.vTCTL);

		// Top left corner
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = 0;
		vFinalPos.y = 0;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsTL.vTCBL, vTexCoordsTL.vTCBR, vTexCoordsTL.vTCTR, vTexCoordsTL.vTCTL);

		// Top right corner
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = pWindow->vDimensions.x - vTextureWindowDimsDiv3.x;
		vFinalPos.y = 0;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsTR.vTCBL, vTexCoordsTR.vTCBR, vTexCoordsTR.vTCTR, vTexCoordsTR.vTCTL);

		// Bottom left corner
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = 0;
		vFinalPos.y = pWindow->vDimensions.y - vTextureWindowDimsDiv3.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsBL.vTCBL, vTexCoordsBL.vTCBR, vTexCoordsBL.vTCTR, vTexCoordsBL.vTCTL);

		// Bottom right corner
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = pWindow->vDimensions.x - vTextureWindowDimsDiv3.x;
		vFinalPos.y = pWindow->vDimensions.y - vTextureWindowDimsDiv3.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsBR.vTCBL, vTexCoordsBR.vTCBR, vTexCoordsBR.vTCTR, vTexCoordsBR.vTCTL);

		// Top edge
		vFinalDims.x = pWindow->vDimensions.x - (vTextureWindowDimsDiv3.x * 2.0f);
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = vTextureWindowDimsDiv3.x;
		vFinalPos.y = 0.0f;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsT.vTCBL, vTexCoordsT.vTCBR, vTexCoordsT.vTCTR, vTexCoordsT.vTCTL);

		// Bottom edge
		vFinalDims.x = pWindow->vDimensions.x - (vTextureWindowDimsDiv3.x * 2.0f);
		vFinalDims.y = vTextureWindowDimsDiv3.y;
		vFinalPos.x = vTextureWindowDimsDiv3.x;
		vFinalPos.y = pWindow->vDimensions.y - vTextureWindowDimsDiv3.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsB.vTCBL, vTexCoordsB.vTCBR, vTexCoordsB.vTCTR, vTexCoordsB.vTCTL);

		// Left edge
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = pWindow->vDimensions.y - (vTextureWindowDimsDiv3.y * 2.0f);
		vFinalPos.x = 0.0f;
		vFinalPos.y = vTextureWindowDimsDiv3.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsL.vTCBL, vTexCoordsL.vTCBR, vTexCoordsL.vTCTR, vTexCoordsL.vTCTL);

		// Right edge
		vFinalDims.x = vTextureWindowDimsDiv3.x;
		vFinalDims.y = pWindow->vDimensions.y - (vTextureWindowDimsDiv3.y * 2.0f);
		vFinalPos.x = pWindow->vDimensions.x - vTextureWindowDimsDiv3.x;
		vFinalPos.y = vTextureWindowDimsDiv3.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsR.vTCBL, vTexCoordsR.vTCBR, vTexCoordsR.vTCTR, vTexCoordsR.vTCTL);

		// Render the background cells
		vertexBuffer.upload();
		vertexBuffer.draw();

		// Render each buttons' background
		Vector2 vButtonOffset;
		if (!pWindow->bWindowIsJustAContainer)
		{
			vButtonOffset.x += vTextureWindowDimsDiv3.x;
			vButtonOffset.y += vTextureWindowDimsDiv3.y;
		}
		Texture* pTextureButtonUp = pManTextures->get2DTexture(pTheme->strTexturenameButton[0], "default");
		Vector2 vTextureButtonUpDims((float)pTextureButtonUp->getWidth(), (float)pTextureButtonUp->getHeight());
		Vector2 vTextureButtonUpDimsDiv3 = vTextureButtonUpDims;
		vTextureButtonUpDimsDiv3.multiply(0.3333333f);
		
		std::map<std::string, GUIButton*>::iterator itb = pWindow->mapGUIButtons.begin();
		while (itb != pWindow->mapGUIButtons.end())
		{
			vertexBuffer.reset();

			// Centre quad
			vFinalPos.x = vTextureButtonUpDimsDiv3.x;
			vFinalPos.y = 0.0f;
			vFinalPos.y += vTextureButtonUpDimsDiv3.y;
			vFinalPos.x += vButtonOffset.x;
			vFinalPos.y += vButtonOffset.y;
			vFinalDims.x = itb->second->vDimensions.x;
			vFinalDims.x -= vTextureButtonUpDimsDiv3.x * 2.0f;
			vFinalDims.y = itb->second->vDimensions.y;
			vFinalDims.y -= vTextureButtonUpDimsDiv3.y * 2.0f;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsC.vTCBL, vTexCoordsC.vTCBR, vTexCoordsC.vTCTR, vTexCoordsC.vTCTL);

			// Top left corner
			vFinalDims.x = vTextureButtonUpDimsDiv3.x;
			vFinalDims.y = vTextureButtonUpDimsDiv3.y;
			vFinalPos.x = 0;
			vFinalPos.y = 0;
			vFinalPos.x += vButtonOffset.x;
			vFinalPos.y += vButtonOffset.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsTL.vTCBL, vTexCoordsTL.vTCBR, vTexCoordsTL.vTCTR, vTexCoordsTL.vTCTL);

			// Top right corner
			vFinalDims.x = vTextureButtonUpDimsDiv3.x;
			vFinalDims.y = vTextureButtonUpDimsDiv3.y;
			vFinalPos.x = itb->second->vDimensions.x - vTextureButtonUpDimsDiv3.x;
			vFinalPos.y = 0;
			vFinalPos.x += vButtonOffset.x;
			vFinalPos.y += vButtonOffset.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsTR.vTCBL, vTexCoordsTR.vTCBR, vTexCoordsTR.vTCTR, vTexCoordsTR.vTCTL);

			// Bottom left corner
			vFinalDims.x = vTextureButtonUpDimsDiv3.x;
			vFinalDims.y = vTextureButtonUpDimsDiv3.y;
			vFinalPos.x = 0;
			vFinalPos.y = itb->second->vDimensions.y - vTextureButtonUpDimsDiv3.y;
			vFinalPos.x += vButtonOffset.x;
			vFinalPos.y += vButtonOffset.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsBL.vTCBL, vTexCoordsBL.vTCBR, vTexCoordsBL.vTCTR, vTexCoordsBL.vTCTL);

			// Bottom right corner
			vFinalDims.x = vTextureButtonUpDimsDiv3.x;
			vFinalDims.y = vTextureButtonUpDimsDiv3.y;
			vFinalPos.x = itb->second->vDimensions.x - vTextureButtonUpDimsDiv3.x;
			vFinalPos.y = itb->second->vDimensions.y - vTextureButtonUpDimsDiv3.y;
			vFinalPos.x += vButtonOffset.x;
			vFinalPos.y += vButtonOffset.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsBR.vTCBL, vTexCoordsBR.vTCBR, vTexCoordsBR.vTCTR, vTexCoordsBR.vTCTL);

			// Top edge
			vFinalDims.x = itb->second->vDimensions.x - (vTextureButtonUpDimsDiv3.x * 2.0f);
			vFinalDims.y = vTextureButtonUpDimsDiv3.y;
			vFinalPos.x = vTextureButtonUpDimsDiv3.x;
			vFinalPos.y = 0.0f;
			vFinalPos.x += vButtonOffset.x;
			vFinalPos.y += vButtonOffset.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsT.vTCBL, vTexCoordsT.vTCBR, vTexCoordsT.vTCTR, vTexCoordsT.vTCTL);

			// Bottom edge
			vFinalDims.x = itb->second->vDimensions.x - (vTextureButtonUpDimsDiv3.x * 2.0f);
			vFinalDims.y = vTextureButtonUpDimsDiv3.y;
			vFinalPos.x = vTextureButtonUpDimsDiv3.x;
			vFinalPos.y = itb->second->vDimensions.y - vTextureButtonUpDimsDiv3.y;
			vFinalPos.x += vButtonOffset.x;
			vFinalPos.y += vButtonOffset.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsB.vTCBL, vTexCoordsB.vTCBR, vTexCoordsB.vTCTR, vTexCoordsB.vTCTL);

			// Left edge
			vFinalDims.x = vTextureButtonUpDimsDiv3.x;
			vFinalDims.y = itb->second->vDimensions.y - (vTextureButtonUpDimsDiv3.y * 2.0f);
			vFinalPos.x = 0.0f;
			vFinalPos.y = vTextureButtonUpDimsDiv3.y;
			vFinalPos.x += vButtonOffset.x;
			vFinalPos.y += vButtonOffset.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsL.vTCBL, vTexCoordsL.vTCBR, vTexCoordsL.vTCTR, vTexCoordsL.vTCTL);

			// Right edge
			vFinalDims.x = vTextureButtonUpDimsDiv3.x;
			vFinalDims.y = itb->second->vDimensions.y - (vTextureButtonUpDimsDiv3.y * 2.0f);
			vFinalPos.x = itb->second->vDimensions.x - vTextureButtonUpDimsDiv3.x;
			vFinalPos.y = vTextureButtonUpDimsDiv3.y;
			vFinalPos.x += vButtonOffset.x;
			vFinalPos.y += vButtonOffset.y;
			vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsR.vTCBL, vTexCoordsR.vTCBR, vTexCoordsR.vTCTR, vTexCoordsR.vTCTL);

			// Render the background cells
			pTextureButtonUp->bind();
			pShader->setInt("texture1", pTextureButtonUp->getID());
			vertexBuffer.upload();
			vertexBuffer.draw();

			itb++;
		}

		// Now render the text for the titlebar
		if (pWindow->strTitlebarText.length())
		{
			TextFont* pTextFont = pManTextFonts->getTextFont(pTheme->strFontnameWindowTitlebar);
			pTextFont->print(pWindow->strTitlebarText,
				(int)pWindow->vPosition.x + (int)vTextureWindowDimsDiv3.x + (int)pTheme->vWindowTitlebarTextOffset.x,
				(int)pWindow->vPosition.y + (int)pTheme->vWindowTitlebarTextOffset.y);
		}
		
		// For each button in window
		itb = pWindow->mapGUIButtons.begin();
		while (itb != pWindow->mapGUIButtons.end())
		{
			TextFont* pTextFont = pManTextFonts->getTextFont(pTheme->strFontnameButton);
			Vector2 vButtonTextPosition;
			vButtonTextPosition.x = (int)pWindow->vPosition.x;
			vButtonTextPosition.x += (int)itb->second->vPosition.x;
			vButtonTextPosition.x += vButtonOffset.x;
			vButtonTextPosition.x += pTheme->vButtonTextOffset.x;
			vButtonTextPosition.x += (int)itb->second->vDimensions.x * 0.5f;
			vButtonTextPosition.y = (int)pWindow->vPosition.y;
			vButtonTextPosition.y += (int)itb->second->vPosition.y;
			vButtonTextPosition.y += vButtonOffset.y;
			vButtonTextPosition.y += pTheme->vButtonTextOffset.y;
			vButtonTextPosition.y += (int)itb->second->vDimensions.y * 0.5f;
			pTextFont->printCentered(itb->second->strText, (int)vButtonTextPosition.x, (int)vButtonTextPosition.y);
			itb++;
		}

	}

	GUITheme* ManagerGUI::addTheme(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.find(name);
		if (mapGUIThemes.end() != itr)
		{
			std::string err("ManagerGUI::addTheme(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object already exists.");
			Log::getPointer()->exception(err);
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

	GUITheme* ManagerGUI::getTheme(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.find(name);
		if (mapGUIThemes.end() == itr)
		{
			std::string err("ManagerGUI::getTheme(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object doesn't exist.");
			Log::getPointer()->exception(err);
		}
		return (GUITheme*)itr->second;
	}

	bool ManagerGUI::getExistsTheme(const std::string& name)
	{
		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.find(name);
		if (itr == mapGUIThemes.end())
			return false;
		return true;
	}

	void ManagerGUI::removeTheme(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.find(name);
		if (mapGUIThemes.end() == itr)
		{
			std::string err("ManagerGUI::removeTheme(\"");
			err.append(name);
			err.append("\") failed because the named object couldn't be found.");
			Log::getPointer()->exception(err);
		}

		// Destroy the resource
		delete itr->second;
		mapGUIThemes.erase(itr);
	}

	GUIWindow* ManagerGUI::addWindow(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, GUIWindow*>::iterator itr = mapGUIWindows.find(name);
		if (mapGUIWindows.end() != itr)
		{
			std::string err("ManagerGUI::addWindow(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object already exists.");
			Log::getPointer()->exception(err);
		}

		// If we get here, we have got to create, then add the resource
		GUIWindow* pNewRes = new GUIWindow();
		mapGUIWindows[name] = pNewRes;

		// Find the object to return a pointer to it
		itr = mapGUIWindows.find(name);
		itr->second->setWindowTitlebarText(name);	// Also set a default name used when rendering the window's titlebar text

		// Add window to z-order
		vecStringWindowZorder.push_back(name);
		return (GUIWindow*)itr->second;
	}

	GUIWindow* ManagerGUI::getWindow(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, GUIWindow*>::iterator itr = mapGUIWindows.find(name);
		if (mapGUIWindows.end() == itr)
		{
			std::string err("ManagerGUI::getWindow(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object doesn't exist.");
			Log::getPointer()->exception(err);
		}
		return (GUIWindow*)itr->second;
	}

	bool ManagerGUI::getExistsWindow(const std::string& name)
	{
		std::map<std::string, GUIWindow*>::iterator itr = mapGUIWindows.find(name);
		if (itr == mapGUIWindows.end())
			return false;
		return true;
	}

	void ManagerGUI::removeWindow(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, GUIWindow*>::iterator itr = mapGUIWindows.find(name);
		if (mapGUIWindows.end() == itr)
		{
			std::string err("ManagerGUI::removeWindow(\"");
			err.append(name);
			err.append("\") failed because the named object couldn't be found.");
			Log::getPointer()->exception(err);
		}

		// Destroy the resource
		delete itr->second;
		mapGUIWindows.erase(itr);
	}

	void ManagerGUI::moveWindowToFront(const std::string& name)
	{
		for (int iWindow = 0; iWindow < vecStringWindowZorder.size(); iWindow++)
		{
			if (vecStringWindowZorder[iWindow].compare(name) == 0)
			{
				if (0 == iWindow)	// Already at front?
				{
					break;
				}
				// Move all windows down one
				while (iWindow > 0)
				{
					vecStringWindowZorder[iWindow] = vecStringWindowZorder[iWindow - 1];
					iWindow--;
				}
				vecStringWindowZorder[0] = name;
				break;
			}
		}
	}

	void ManagerGUI::setCurrentTheme(const std::string& name)
	{
		strCurrentTheme = name;
	}

	void ManagerGUI::loadAllThemes(void)
	{
		ManagerTextures* pManTextures = ManagerTextures::getPointer();
		ManagerTextFonts* pManTextFonts = ManagerTextFonts::getPointer();

		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.begin();
		while (itr != mapGUIThemes.end())
		{
			if (!itr->second->bLoaded)
			{
				itr->second->bLoaded = true;
				pManTextures->add2DTexture(itr->second->strTexturenameWindow, itr->second->strTexturenameWindow, "default", true, TextureFiltering::linear);
				pManTextures->add2DTexture(itr->second->strTexturenameButton[0], itr->second->strTexturenameButton[0], "default", true, TextureFiltering::linear);
				pManTextures->add2DTexture(itr->second->strTexturenameButton[1], itr->second->strTexturenameButton[1], "default", true, TextureFiltering::linear);
				pManTextures->add2DTexture(itr->second->strTexturenameButton[2], itr->second->strTexturenameButton[2], "default", true, TextureFiltering::linear);
				pManTextFonts->addTextFont(itr->second->strFontnameWindowTitlebar);
				pManTextFonts->addTextFont(itr->second->strFontnameButton);
			}
			itr++;
		}
		pManTextFonts->loadAll();
	}
}