#include "precompiled_header.h"
#include "gui_window.h"
#include "../core/log.h"

namespace Nexus
{

	GUIWindow::GUIWindow()
	{
		bEnabled = true;
		bWindowIsJustAContainer = false;
		vDimensions.set(320, 240);
		bBeingMoved = false;
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
}