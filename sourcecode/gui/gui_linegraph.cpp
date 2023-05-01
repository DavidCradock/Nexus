#include "precompiled_header.h"
#include "gui_linegraph.h"
#include "../managers/managerGUI.h"
#include "../managers/managerShaders.h"
#include "../managers/managerTextures.h"
#include "../managers/managerTextFonts.h"
#include "../graphics/vertexBuffer.h"
#include "../graphics/renderDevice.h"

namespace Nexus
{

	GUILineGraph::GUILineGraph()
	{
		vDimensions.set(100, 100);
		vPosition.set(0, 0);
		strTextX = "X axis";
		strTextY = "Y axis";
		iMaxNumberValues = 100;
	}

	// Adds a new value to the linegraph
	void GUILineGraph::addValue(float fValue)
	{
		listValues.push_front(fValue);
		if (listValues.size() > iMaxNumberValues)
			listValues.pop_back();
	}

	// Sets maximum number of values
	void GUILineGraph::setMaxNumValues(int iMaxNumValues)
	{
		if (iMaxNumValues < 1)
			iMaxNumValues = 1;
		iMaxNumberValues = iMaxNumValues;
	}

	void GUILineGraph::removeAllValues(void)
	{
		listValues.clear();
	}

	void GUILineGraph::render(GUIWindow* pWindow)
	{
		RenderDevice* pRD = RenderDevice::getPointer();
		ManagerGUI* pManGUI = ManagerGUI::getPointer();
		ManagerTextFonts* pManTextFonts = ManagerTextFonts::getPointer();
		ManagerShaders* pManShaders = ManagerShaders::getPointer();
//		Shader* pShader = pManShaders->getShader("gui_line");
		GUITheme* pTheme = pManGUI->getCurrentTheme();
		TextFont* pTextFont = pManTextFonts->getTextFont(pTheme->strFontnameLinegraph);
		ManagerTextures* pManTextures = ManagerTextures::getPointer();
		Texture* pTextureWindow = pManTextures->get2DTexture(pTheme->strTexturenameWindow);
		Vector2 vTextureWindowDims((float)pTextureWindow->getWidth(), (float)pTextureWindow->getHeight());
		Vector2 vTextureWindowDimsDiv3 = vTextureWindowDims;
		vTextureWindowDimsDiv3.multiply(0.3333333f);
		Vector2 vPos = pWindow->getWindowPosition();
		vPos.x += vTextureWindowDimsDiv3.x;
		vPos.y += vTextureWindowDimsDiv3.y;

		// Print background
		vPos.x += vPosition.x;
		vPos.y += vPosition.y;
		pManTextures->disableTexturing();
//		pShader->setInt("texture1", 0);
		VertexBuffer vb;
		vb.addQuad(vPos, vDimensions, Vector4(pTheme->linegraphBackgroundColour.r, pTheme->linegraphBackgroundColour.g, pTheme->linegraphBackgroundColour.b, pTheme->linegraphBackgroundColour.a));
		vb.upload();
		vb.draw();

		// Print horizontal text
		pTextFont->printCentered(strTextX,
			(int)vPosition.x + int(vDimensions.x * 0.5f) + (int)vPos.x,
			(int)vPosition.y + int(vDimensions.y) + (int)vPos.y - (int)vTextureWindowDimsDiv3.y,
			pRD->getWindowWidth(), pRD->getWindowHeight(),
			pTheme->linegraphTextColour);
		// Print vertical text
		pTextFont->print(strTextY,
			(int)vPosition.x + (int)vPos.x,
			(int)vPosition.y + int(vDimensions.y * 0.5f) + (int)vPos.y - (int)vTextureWindowDimsDiv3.y,
			pRD->getWindowWidth(), pRD->getWindowHeight(),
			pTheme->linegraphTextColour);




	}

}