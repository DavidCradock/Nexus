#include "precompiled_header.h"
#include "gui_text.h"
#include "../managers/managerGUI.h"
#include "../managers/managerInputDevices.h"
#include "../managers/managerShaders.h"
#include "../managers/managerTextFonts.h"
#include "../managers/managerTextures.h"
#include "../graphics/vertexBuffer.h"

namespace Nexus
{

	GUIText::GUIText()
	{
		vDimensions.set(100, 100);
		strText.append("text");
	}

	void GUIText::render(GUIWindow* pWindow)
	{
		ManagerGUI* pManGUI = ManagerGUI::getPointer();
		ManagerTextures* pManTextures = ManagerTextures::getPointer();
		ManagerTextFonts* pManTextFonts = ManagerTextFonts::getPointer();
		GUITheme* pTheme = pManGUI->getCurrentTheme();
		Texture* pTextureWindow = pManTextures->get2DTexture(pTheme->strTexturenameWindow);
		Vector2 vTextureWindowDims((float)pTextureWindow->getWidth(), (float)pTextureWindow->getHeight());
		Vector2 vTextureWindowDimsDiv3 = vTextureWindowDims;
		vTextureWindowDimsDiv3.multiply(0.3333333f);
		Vector2 vWindowPos = pWindow->getWindowPosition();

		Vector2 vOffset;
		if (false == pWindow->getWindowIsContainer())
		{
			vOffset.x += vTextureWindowDimsDiv3.x;
			vOffset.y += vTextureWindowDimsDiv3.y;
		}
		vOffset.x += vWindowPos.x;
		vOffset.y += vWindowPos.y;

		// Compute top left position of the text
		Vector2 vTextPosition;
		vTextPosition.x = vPosition.x;
		vTextPosition.x += vOffset.x;
		vTextPosition.y = vPosition.y;
		vTextPosition.y += vOffset.y;

		// Split string into words seperated by space
		std::istringstream ss(strText);	// Copy the text into istringstream
		std::string strWord;	// Holds each word
		while (getline(ss, strWord, ' '))	// For each word, place into strWord
		{
			// Do stuff
		}

		TextFont* pTextFont = pManTextFonts->getTextFont(pTheme->strFontnameText);

		pTextFont->print(strText, (int)vTextPosition.x, (int)vTextPosition.y, pTheme->textColour);

	}

}