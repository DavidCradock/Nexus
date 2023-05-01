#include "precompiled_header.h"
#include "managerGUI.h"
#include "../core/log.h"
#include "managerTextFonts.h"
#include "managerTextures.h"

namespace Nexus
{
	ManagerGUI::ManagerGUI()
	{
		bMouseIsOverGUI = false;
		bWindowBeingMoved = false;

		// Create frame statistics window
		GUIWindow *pWindow = addWindow("Frame Statistics");
		pWindow->setWindowPosition(Vector2(0, 0));
		pWindow->setWindowDimensions(640, 480);

		GUIText *pText = pWindow->addText("FPSCurrent");
		pText->setPosition(Vector2(0, 0));
		pText->setDimensions(640, 480);
		
		pText = pWindow->addText("FPSSmoothed");
		pText->setPosition(Vector2(0, 20));
		pText->setDimensions(640, 480);

		GUILineGraph* pLineGraph = pWindow->addLineGraph("FPS");
		pLineGraph->setPosition(0, 60);
		pLineGraph->setDimenions(640, 380);
		pLineGraph->setTextX("Time");
		pLineGraph->setTextY("FPS");
		fTimeToAddValueToFPSLineGraph = 0.0f;
	}

	void ManagerGUI::update(void)
	{
		// If there are no windows, do nothing
		if (!mapGUIWindows.size())
			return;

		// Theme not set?
		if (!strCurrentTheme.length())
			Log::getPointer()->exception("ManagerGUI::render() failed as currently set theme is not set.");

		// Get currently set theme dims
		GUITheme* pTheme = getTheme(strCurrentTheme);

		// Set GUIWindow "Frame Statistics" text
		timing.update();
		GUIWindow* pWindow = getWindow("Frame Statistics");
		GUIText* pText = pWindow->getText("FPSCurrent");
		std::string strTextFPS("Framerate Current: ");
		strTextFPS.append(std::to_string((int)timing.getStatFPS()));
		pText->setText(strTextFPS);
		pText = pWindow->getText("FPSSmoothed");
		strTextFPS = "Framerate Smoothed: ";
		strTextFPS.append(std::to_string((int)timing.getStatFPSS()));
		pText->setText(strTextFPS);
		// Update line graph FPS
		fTimeToAddValueToFPSLineGraph += (float)timing.getSecPast();
		if (fTimeToAddValueToFPSLineGraph >= 1.0f)
		{
			fTimeToAddValueToFPSLineGraph -= 1.0f;
			GUILineGraph* pLineGraph = pWindow->getLineGraph("FPS");
			pLineGraph->addValue((float)timing.getStatFPS());
		}

		// For each window
		std::map<std::string, GUIWindow*>::iterator itw = mapGUIWindows.begin();
		bMouseIsOverGUI = false;
		bWindowBeingMoved = false;
		while (itw != mapGUIWindows.end())
		{
			if (itw->second->update(itw->first))
				bMouseIsOverGUI = true;
			itw++;
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
			itw->second->render();
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

	std::string ManagerGUI::getCurrentThemeName(void)
	{
		return strCurrentTheme;
	}

	GUITheme* ManagerGUI::getCurrentTheme(void)
	{
		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.find(strCurrentTheme);
		if (itr == mapGUIThemes.end())
		{
			Log::getPointer()->exception("ManagerGUI::getCurrentTheme() failed to find currently set GUITheme");
		}
		return itr->second;
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
				pManTextFonts->addTextFont(itr->second->strFontnameText);
				pManTextFonts->addTextFont(itr->second->strFontnameLinegraph);
			}
			itr++;
		}
		pManTextFonts->loadAll();
	}
}