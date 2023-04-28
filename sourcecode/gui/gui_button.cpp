#include "precompiled_header.h"
#include "gui_button.h"
#include "../managers/managerGUI.h"
#include "../managers/managerInputDevices.h"
#include "../managers/managerShaders.h"
#include "../managers/managerTextFonts.h"
#include "../managers/managerTextures.h"
#include "../graphics/vertexBuffer.h"

namespace Nexus
{

	GUIButton::GUIButton()
	{
		vDimensions.set(100, 40);
		bMouseOver = false;
		bMouseDown = false;
		bClickedOn = false;

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

	void GUIButton::update(GUIWindow* pWindow)
	{
		ManagerGUI* pManGUI = ManagerGUI::getPointer();
		ManagerTextures* pManTextures = ManagerTextures::getPointer();
		GUITheme* pTheme = pManGUI->getCurrentTheme();
		Texture* pTextureWindow = pManTextures->get2DTexture(pTheme->strTexturenameWindow);
		Vector2 vTextureWindowDims((float)pTextureWindow->getWidth(), (float)pTextureWindow->getHeight());
		Vector2 vTextureWindowDimsDiv3 = vTextureWindowDims;
		vTextureWindowDimsDiv3.multiply(0.3333333f);
		Vector2 vWindowPos = pWindow->getWindowPosition();

		Vector2 vButtonPos = pWindow->getWindowPosition();
		vButtonPos.x += vPosition.x;
		vButtonPos.y += vPosition.y;
		Vector2 vButtonDims = vDimensions;
		if (false == pWindow->getWindowIsContainer())
		{ 
			vButtonPos.x += vTextureWindowDimsDiv3.x;
			vButtonPos.y += vTextureWindowDimsDiv3.y;
		}

		// Mouse info
		ManagerInputDevices* pManInputDevices = ManagerInputDevices::getPointer();
		Vector2 vMousePosCurrent = pManInputDevices->mouse.getCursorPos();

		// If the mouse was previously pressed and over but now moue button is up, that's a click!
		bClickedOn = false;
		if (!pManInputDevices->mouse.leftButDown())
		{
			if (bMouseDown)
			{
				bClickedOn = true;
			}
		}
		bMouseOver = false;
		bMouseDown = false;
		

		if (vMousePosCurrent.x > vButtonPos.x)
		{
			if (vMousePosCurrent.x < vButtonPos.x + vButtonDims.x)
			{
				if (vMousePosCurrent.y > vButtonPos.y)
				{
					if (vMousePosCurrent.y < vButtonPos.y + vButtonDims.y)
					{
						bMouseOver = true;


						if (pManInputDevices->mouse.leftButDown())
						{
							bMouseDown = true;
						}
						
					}
				}
			}
		}
	}

	void GUIButton::render(GUIWindow* pWindow)
	{
		ManagerGUI* pManGUI = ManagerGUI::getPointer();
		ManagerTextures* pManTextures = ManagerTextures::getPointer();
		ManagerTextFonts* pManTextFonts = ManagerTextFonts::getPointer();
		ManagerShaders* pManShaders = ManagerShaders::getPointer();
		GUITheme* pTheme = pManGUI->getCurrentTheme();
		Texture* pTextureWindow = pManTextures->get2DTexture(pTheme->strTexturenameWindow);
		Vector2 vTextureWindowDims((float)pTextureWindow->getWidth(), (float)pTextureWindow->getHeight());
		Vector2 vTextureWindowDimsDiv3 = vTextureWindowDims;
		vTextureWindowDimsDiv3.multiply(0.3333333f);
		Vector2 vWindowPos = pWindow->getWindowPosition();

		// Render background
		Vector2 vButtonOffset;
		if (false == pWindow->getWindowIsContainer())
		{
			vButtonOffset.x += vTextureWindowDimsDiv3.x;
			vButtonOffset.y += vTextureWindowDimsDiv3.y;
		}
		vButtonOffset.x += vWindowPos.x;
		vButtonOffset.y += vWindowPos.y;

		Texture* pTextureButtonUp = pManTextures->get2DTexture(pTheme->strTexturenameButton[0], "default");
		Vector2 vTextureButtonUpDims((float)pTextureButtonUp->getWidth(), (float)pTextureButtonUp->getHeight());
		Vector2 vTextureButtonUpDimsDiv3 = vTextureButtonUpDims;
		vTextureButtonUpDimsDiv3.multiply(0.3333333f);

		VertexBuffer vertexBuffer;
		vertexBuffer.reset();
		Shader* pShader = pManShaders->getShader("gui");
		Vector2 vFinalPos;
		Vector2 vFinalDims;

		// Centre quad
		vFinalPos.x = vTextureButtonUpDimsDiv3.x;
		vFinalPos.y = 0.0f;
		vFinalPos.y += vTextureButtonUpDimsDiv3.y;
		vFinalPos.x += vButtonOffset.x;
		vFinalPos.y += vButtonOffset.y;
		vFinalDims.x = vDimensions.x;
		vFinalDims.x -= vTextureButtonUpDimsDiv3.x * 2.0f;
		vFinalDims.y = vDimensions.y;
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
		vFinalPos.x = vDimensions.x - vTextureButtonUpDimsDiv3.x;
		vFinalPos.y = 0;
		vFinalPos.x += vButtonOffset.x;
		vFinalPos.y += vButtonOffset.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsTR.vTCBL, vTexCoordsTR.vTCBR, vTexCoordsTR.vTCTR, vTexCoordsTR.vTCTL);

		// Bottom left corner
		vFinalDims.x = vTextureButtonUpDimsDiv3.x;
		vFinalDims.y = vTextureButtonUpDimsDiv3.y;
		vFinalPos.x = 0;
		vFinalPos.y = vDimensions.y - vTextureButtonUpDimsDiv3.y;
		vFinalPos.x += vButtonOffset.x;
		vFinalPos.y += vButtonOffset.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsBL.vTCBL, vTexCoordsBL.vTCBR, vTexCoordsBL.vTCTR, vTexCoordsBL.vTCTL);

		// Bottom right corner
		vFinalDims.x = vTextureButtonUpDimsDiv3.x;
		vFinalDims.y = vTextureButtonUpDimsDiv3.y;
		vFinalPos.x = vDimensions.x - vTextureButtonUpDimsDiv3.x;
		vFinalPos.y = vDimensions.y - vTextureButtonUpDimsDiv3.y;
		vFinalPos.x += vButtonOffset.x;
		vFinalPos.y += vButtonOffset.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsBR.vTCBL, vTexCoordsBR.vTCBR, vTexCoordsBR.vTCTR, vTexCoordsBR.vTCTL);

		// Top edge
		vFinalDims.x = vDimensions.x - (vTextureButtonUpDimsDiv3.x * 2.0f);
		vFinalDims.y = vTextureButtonUpDimsDiv3.y;
		vFinalPos.x = vTextureButtonUpDimsDiv3.x;
		vFinalPos.y = 0.0f;
		vFinalPos.x += vButtonOffset.x;
		vFinalPos.y += vButtonOffset.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsT.vTCBL, vTexCoordsT.vTCBR, vTexCoordsT.vTCTR, vTexCoordsT.vTCTL);

		// Bottom edge
		vFinalDims.x = vDimensions.x - (vTextureButtonUpDimsDiv3.x * 2.0f);
		vFinalDims.y = vTextureButtonUpDimsDiv3.y;
		vFinalPos.x = vTextureButtonUpDimsDiv3.x;
		vFinalPos.y = vDimensions.y - vTextureButtonUpDimsDiv3.y;
		vFinalPos.x += vButtonOffset.x;
		vFinalPos.y += vButtonOffset.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsB.vTCBL, vTexCoordsB.vTCBR, vTexCoordsB.vTCTR, vTexCoordsB.vTCTL);

		// Left edge
		vFinalDims.x = vTextureButtonUpDimsDiv3.x;
		vFinalDims.y = vDimensions.y - (vTextureButtonUpDimsDiv3.y * 2.0f);
		vFinalPos.x = 0.0f;
		vFinalPos.y = vTextureButtonUpDimsDiv3.y;
		vFinalPos.x += vButtonOffset.x;
		vFinalPos.y += vButtonOffset.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsL.vTCBL, vTexCoordsL.vTCBR, vTexCoordsL.vTCTR, vTexCoordsL.vTCTL);

		// Right edge
		vFinalDims.x = vTextureButtonUpDimsDiv3.x;
		vFinalDims.y = vDimensions.y - (vTextureButtonUpDimsDiv3.y * 2.0f);
		vFinalPos.x = vDimensions.x - vTextureButtonUpDimsDiv3.x;
		vFinalPos.y = vTextureButtonUpDimsDiv3.y;
		vFinalPos.x += vButtonOffset.x;
		vFinalPos.y += vButtonOffset.y;
		vertexBuffer.addQuad(vFinalPos, vFinalDims, Vector3(1.0f, 1.0f, 1.0f), vTexCoordsR.vTCBL, vTexCoordsR.vTCBR, vTexCoordsR.vTCTR, vTexCoordsR.vTCTL);

		// Render the background cells
		pTextureButtonUp->bind();
		pShader->setInt("texture1", pTextureButtonUp->getID());
		vertexBuffer.upload();
		vertexBuffer.draw();


		TextFont* pTextFont = pManTextFonts->getTextFont(pTheme->strFontnameButton);
		Vector2 vButtonTextPosition;
		vButtonTextPosition.x = vPosition.x;
		vButtonTextPosition.x += vPosition.x;
		vButtonTextPosition.x += vButtonOffset.x;
		vButtonTextPosition.x += pTheme->vButtonTextOffset.x;
		vButtonTextPosition.x += vDimensions.x * 0.5f;
		vButtonTextPosition.y = vPosition.y;
		vButtonTextPosition.y += vPosition.y;
		vButtonTextPosition.y += vButtonOffset.y;
		vButtonTextPosition.y += pTheme->vButtonTextOffset.y;
		vButtonTextPosition.y += vDimensions.y * 0.5f;
		Colourf textColour = pTheme->buttonTextColour[0];
		if (bMouseOver)
			textColour = pTheme->buttonTextColour[1];
		if (bMouseDown)
			textColour = pTheme->buttonTextColour[2];

		pTextFont->printCentered(strText, (int)vButtonTextPosition.x, (int)vButtonTextPosition.y, textColour);

	}

}