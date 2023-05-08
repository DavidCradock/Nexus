#include "precompiled_header.h"
#include "GUIManager.h"
#include "../core/log.h"
#include "managers.h"

namespace Nexus
{
	GUIManager::GUIManager()
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

		// Don't show Frame statistics window by default
		pWindow->setWindowEnabled(false);
	}

	void GUIManager::update(void)
	{
		// If there are no windows, do nothing
		if (!mapGUIWindows.size())
			return;

		// Theme not set?
		if (!strCurrentTheme.length())
			Log::getPointer()->exception("GUIManager::render() failed as currently set theme is not set.");

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
		if (fTimeToAddValueToFPSLineGraph >= 0.1f)
		{
			fTimeToAddValueToFPSLineGraph -= 0.1f;
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

	void GUIManager::render(void)
	{
		// If there are no windows, do nothing
		if (!mapGUIWindows.size())
			return;

		if (!strCurrentTheme.length())
			Log::getPointer()->exception("GUIManager::render() failed as currently set theme is not set.");

		// For each window, using z-ordering with rendering back most first
		for (int iWindow = (int)vecStringWindowZorder.size() - 1; iWindow >= 0; iWindow--)
		{
			std::map<std::string, GUIWindow*>::iterator itw = mapGUIWindows.find(vecStringWindowZorder[iWindow]);
			itw->second->render();
		}
	}

	GUITheme* GUIManager::addTheme(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.find(name);
		if (mapGUIThemes.end() != itr)
		{
			std::string err("GUIManager::addTheme(\"");
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
			Log::getPointer()->exception(err);
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
			Log::getPointer()->exception(err);
		}

		// Destroy the resource
		delete itr->second;
		mapGUIThemes.erase(itr);
	}

	GUIWindow* GUIManager::addWindow(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, GUIWindow*>::iterator itr = mapGUIWindows.find(name);
		if (mapGUIWindows.end() != itr)
		{
			std::string err("GUIManager::addWindow(\"");
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
			Log::getPointer()->exception(err);
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
			Log::getPointer()->exception(err);
		}

		// Destroy the resource
		delete itr->second;
		mapGUIWindows.erase(itr);
	}

	void GUIManager::moveWindowToFront(const std::string& name)
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

	void GUIManager::setCurrentTheme(const std::string& name)
	{
		strCurrentTheme = name;
	}

	std::string GUIManager::getCurrentThemeName(void)
	{
		return strCurrentTheme;
	}

	GUITheme* GUIManager::getCurrentTheme(void)
	{
		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.find(strCurrentTheme);
		if (itr == mapGUIThemes.end())
		{
			Log::getPointer()->exception("GUIManager::getCurrentTheme() failed to find currently set GUITheme");
		}
		return itr->second;
	}

	void GUIManager::loadAllThemes(void)
	{
		Managers* pMan = Managers::getPointer();

		std::map<std::string, GUITheme*>::iterator itr = mapGUIThemes.begin();
		while (itr != mapGUIThemes.end())
		{
			if (!itr->second->bLoaded)
			{
				itr->second->bLoaded = true;
				pMan->textures->add(itr->second->strTexturenameWindow, itr->second->strTexturenameWindow, "default", true, TextureFiltering::linear);
				pMan->textures->add(itr->second->strTexturenameButton[0], itr->second->strTexturenameButton[0], "default", true, TextureFiltering::linear);
				pMan->textures->add(itr->second->strTexturenameButton[1], itr->second->strTexturenameButton[1], "default", true, TextureFiltering::linear);
				pMan->textures->add(itr->second->strTexturenameButton[2], itr->second->strTexturenameButton[2], "default", true, TextureFiltering::linear);
				pMan->textFonts->add(itr->second->strFontnameWindowTitlebar);
				pMan->textFonts->add(itr->second->strFontnameButton);
				pMan->textFonts->add(itr->second->strFontnameText);
				pMan->textFonts->add(itr->second->strFontnameLinegraph);
			}
			itr++;
		}
		pMan->textFonts->loadAll();
	}
}