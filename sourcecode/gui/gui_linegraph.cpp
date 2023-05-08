#include "precompiled_header.h"
#include "gui_linegraph.h"
#include "../managers/managers.h"
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
		fMaxValue = 0.0f;
		fMinValue = 9999999.0f;
	}

	// Adds a new value to the linegraph
	void GUILineGraph::addValue(float fValue)
	{
		listValues.push_front(fValue);
		if (listValues.size() > iMaxNumberValues)
			listValues.pop_back();
		// Compute fMaxValue
		std::list<float>::iterator it = listValues.begin();
		fMaxValue = 0.0f;
		fMinValue = 9999999.0f;
		while (it != listValues.end())
		{
			if (*it > fMaxValue)
				fMaxValue = *it;
			if (*it < fMinValue)
				fMinValue = *it;
			it++;
		}
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
		Managers* pMan = Managers::getPointer();
		Shader* pShader = pMan->shaders->get("default");
		GUITheme* pTheme = pMan->gui->getCurrentTheme();
		TextFont* pTextFont = pMan->textFonts->get(pTheme->strFontnameLinegraph);
		Texture* pTextureWindow = pMan->textures->get(pTheme->strTexturenameWindow);
		Vector2 vTextureWindowDims((float)pTextureWindow->getWidth(), (float)pTextureWindow->getHeight());
		Vector2 vTextureWindowDimsDiv3 = vTextureWindowDims;
		vTextureWindowDimsDiv3.multiply(0.3333333f);
		Vector2 vPos = pWindow->getWindowPosition();
		vPos.x += vTextureWindowDimsDiv3.x;
		vPos.y += vTextureWindowDimsDiv3.y;

		// Print background
		vPos.x += vPosition.x;
		vPos.y += vPosition.y;
		pMan->textures->disableTexturing();
//		pShader->setInt("texture1", 0);
		VertexBuffer vb;
		vb.addQuad(vPos, vDimensions, Vector4(pTheme->linegraphBackgroundColour.r, pTheme->linegraphBackgroundColour.g, pTheme->linegraphBackgroundColour.b, pTheme->linegraphBackgroundColour.a));
		vb.upload();
		vb.draw();

		Texture* pTextureWhite = pMan->textures->get("white_32x32", "default");
		pTextureWhite->bind();
		pShader->use();
		pShader->setInt("texture1", pTextureWhite->getID());
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		Vector2 vRTDims;
		vRTDims.x = (float)pRD->getWindowWidth();
		vRTDims.y = (float)pRD->getWindowHeight();
		Matrix matrixOrtho;
		matrixOrtho.setOrthographic(0.0f, vRTDims.x, 0.0f, vRTDims.y);
		Matrix matrixTransform;
		matrixTransform.setIdentity();
		pShader->setMat4("transform", matrixOrtho * matrixTransform);

		vb.reset();
		if (listValues.size() > 1)
		{
			float fXoffset = vDimensions.x / float(listValues.size()-1);	// Amount to move right for each line

			std::list<float>::iterator it1 = listValues.begin();
			std::list<float>::iterator it2 = listValues.begin();
			it2++;

			Vector2 vLinePointStart = vPos;
			Vector2 vLinePointEnd = vPos;
			vLinePointEnd.x += fXoffset;

			while (it2 != listValues.end())
			{			
				vLinePointStart.y = (vPos.y + vDimensions.y) - ((*it1 / fMaxValue) * vDimensions.y);
				vLinePointEnd.y = (vPos.y + vDimensions.y) - ((*it2 / fMaxValue) * vDimensions.y);
				vb.addLine(vLinePointStart, vLinePointEnd, 3.0f);
				vLinePointStart.x += fXoffset;
				vLinePointEnd.x += fXoffset;
				it1++;
				it2++;
			}
		}
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