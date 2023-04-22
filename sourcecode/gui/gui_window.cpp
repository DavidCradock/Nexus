#include "precompiled_header.h"
#include "gui_window.h"
#include "../core/log.h"

namespace Nexus
{

	GUIWindow::GUIWindow()
	{
		bEnabled = true;
		vDimensions.set(320, 240);

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

	void GUIWindow::setWindowEnabled(bool bEnabledIn)
	{
		bEnabled = bEnabled;
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

	GUIButton* GUIWindow::createButton(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, GUIButton*>::iterator itr = mapGUIButtons.find(name);
		if (mapGUIButtons.end() != itr)
		{
			std::string err("GUIWindow::createButton(\"");
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
}