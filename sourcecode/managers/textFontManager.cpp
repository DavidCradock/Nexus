#include "precompiled_header.h"
#include "textFontManager.h"
#include "managers.h"
#include "../core/log.h"
#include "../graphics/renderDevice.h"
#include "../graphics/image.h"
#include "../graphics/vertexBuffer.h"
#include "../graphics/shader.h"

namespace Nexus
{
	TextFont* TextFontManager::add(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, TextFont*>::iterator itr = mapTextFonts.find(name);
		if (mapTextFonts.end() != itr)
		{
			itr->second->refCount++;
			return (TextFont*)itr->second;
		}

		// If we get here, we have got to create, then add the resource
		TextFont* pNewRes = new TextFont();
		mapTextFonts[name] = pNewRes;

		// Find the object to return a pointer to it
		itr = mapTextFonts.find(name);
		return (TextFont*)itr->second;
	}

	TextFont* TextFontManager::get(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, TextFont*>::iterator itr = mapTextFonts.find(name);
		if (mapTextFonts.end() == itr)
		{
			std::string err("TextFontManager::get(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object doesn't exist.");
			Log::getPointer()->exception(err);
		}
		return (TextFont*)itr->second;
	}

	bool TextFontManager::getExists(const std::string& name)
	{
		std::map<std::string, TextFont*>::iterator itr = mapTextFonts.find(name);
		if (itr == mapTextFonts.end())
			return false;
		return true;
	}

	void TextFontManager::remove(const std::string& name)
	{
		// Resource doesn't exist in the group?
		std::map<std::string, TextFont*>::iterator itr = mapTextFonts.find(name);
		if (mapTextFonts.end() == itr)
		{
			std::string err("TextFontManager::remove(\"");
			err.append(name);
			err.append("\") failed because the named object couldn't be found.");
			Log::getPointer()->exception(err);
		}

		itr->second->refCount--;
		// If the reference count is now at zero
		if (itr->second->refCount <= 0)
		{
			// Destroy the resource
			delete itr->second;
			mapTextFonts.erase(itr);
		}
	}

	void TextFontManager::loadAll(void)
	{
		std::map<std::string, TextFont*>::iterator itr = mapTextFonts.begin();
		// If nothing to load
		if (itr == mapTextFonts.end())
			return;
		while (itr != mapTextFonts.end())
		{
			itr->second->load(itr->first);
			itr++;
		}
	}

	void TextFontManager::buildFontFiles(const std::string& strOutputBaseName, const std::string& strFontName, unsigned int iFontHeight, bool bAntialiased, bool bBold, bool bItalic, bool bUnderlined, bool bStrikeout)
	{
		// We need to use Windows GDI text rendering to obtain character spacing and dimension information.
		// We then take that, create textures holding each type of font (Normal, Bold, Italic, Underlined and Strikeout) and save them to disk.
		// We also save all the character information to a .fnt file
		// This does NOT initialise any textures or fonts ready for use, it just builds the required files for the TextFont object in it's load method.

		HFONT font;
		HFONT oldFont;
		HDC hDC = 0;
		TEXTMETRIC tm;

		int iAntiAliased = ANTIALIASED_QUALITY;
		if (!bAntialiased)
			iAntiAliased = NONANTIALIASED_QUALITY;
		int iActualHeight = -(int)iFontHeight;

		int iWeight = 400;	// 800 for BOLD
		if (bBold)
			iWeight = 800;

		// Attempt to generate font
		font = CreateFont(iActualHeight,// Height Of Font
			0,							// Width Of Font
			0,							// Angle Of Escapement
			0,							// Orientation Angle
			iWeight,					// Font Weight 400 for normal, 800 to bold
			bItalic,					// Italic
			bUnderlined,				// Underline
			bStrikeout,					// Strikeout
			ANSI_CHARSET,				// Character Set Identifier
			OUT_TT_PRECIS,				// Output Precision
			CLIP_DEFAULT_PRECIS,		// Clipping Precision
			iAntiAliased,				// Output Quality. Either ANTIALIASED_QUALITY or NONANTIALIASED_QUALITY
			FF_DONTCARE | DEFAULT_PITCH,// Family And Pitch
			strFontName.c_str());		// Font Name
		if (font == NULL)	// Unable to generate new font?
		{
			Log::getPointer()->exception("TextFontManager::buildFontFiles() failed upon call to Win32 API call CreateFont()");
		}

		hDC = CreateCompatibleDC(0);
		if (0 == hDC)
		{
			DeleteObject(font);
			Log::getPointer()->exception("TextFontManager::buildFontFiles() failed upon call to Win32 API call CreateCompatibleDC()");
		}

		SetTextColor(hDC, RGB(255, 255, 255));
		SetBkColor(hDC, RGB(0, 0, 0));
		SetBkMode(hDC, TRANSPARENT);
		SetMapMode(hDC, MM_TEXT);
		SetTextAlign(hDC, TA_TOP | TA_LEFT);
		oldFont = (HFONT)SelectObject(hDC, font);	// Store oldFont to return it when we're done.

		SelectObject(hDC, font);
		GetTextMetrics(hDC, &tm);
		float fMaxHeight = (float)tm.tmHeight;
		TextFont::CharDesc charDesc[256];
		ABCFLOAT abcf;
		// Get information for the font
		SelectObject(hDC, font);
		for (int i = 0; i < 256; ++i)
		{
			// Get dims of each character
			GetCharABCWidthsFloat(hDC, i, i, &abcf);
			charDesc[i].fABCa = abcf.abcfA;
			charDesc[i].fABCb = abcf.abcfB;
			charDesc[i].fABCc = abcf.abcfC;

			// Add some spacing (Due to possible antialiasing)
			charDesc[i].fABCa -= 1;
			charDesc[i].fABCb += 2;
			charDesc[i].fABCc -= 1;
		}

		Vector2 vTLPospx[256];	// Top left position of character within texture, in pixels

		// Now compute required size of power of 2 texture to store all characters
		Vector2 vFinalTextureDims(128, 128);

		// Find final texture dimensions required for normal
		bool bFoundFinalTextureDims = false;
		while (false == bFoundFinalTextureDims)
		{
			bool bAllCharsFit = true;
			float fXCurPos = 0;
			float fYCurPos = 0;
			Vector2 vTexSpan;
			vTexSpan.x = 1.0f / vFinalTextureDims.x;
			vTexSpan.y = 1.0f / vFinalTextureDims.y;
			// Go through each char, trying to fit into currently sized texture dimensions.
			// If at any time, we discover that not all of the characters can not fit,
			// we simply increase the texture size and try again.
			for (int i = 0; i < 256; ++i)
			{
				// From current position, would current character fit?
				if (fXCurPos + charDesc[i].fABCb >= vFinalTextureDims.x)
				{
					// If we get here, the current character doesn't fit on current row
					// Move down to the next row, left edge, as might be enough room on next row
					fXCurPos = 0;
					fYCurPos += fMaxHeight;
					// Would char fit along X axis?
					if (charDesc[i].fABCb >= vFinalTextureDims.x)
					{
						// The width of the texture isn't even big enough to fit this single character
						bAllCharsFit = false;
						break;
					}
					// Is there enough room along Y axis?
					if (fYCurPos + fMaxHeight >= vFinalTextureDims.y)
					{
						bAllCharsFit = false;
						break;	// Not enough room along Y axis
					}
				}
				// If we get here, then char fits, simply store location and increase current X position
				vTLPospx[i].x = float(fXCurPos);
				vTLPospx[i].y = float(fYCurPos);
				fXCurPos += charDesc[i].fABCb;

			}	// For each character image

			// If bAllCharsFit = false. Then, we need to re-size texture dimensions
			if (false == bAllCharsFit)
			{
				if (vFinalTextureDims.y < vFinalTextureDims.x)
					vFinalTextureDims.y *= 2;
				else
					vFinalTextureDims.x *= 2;
			}
			else	// If bAllCharsFit = true, then all chars fit to texture
				bFoundFinalTextureDims = true;
		}

		// Create DC bitmap to put char images into for Normal font
		BITMAPINFO	bmi;
		HBITMAP		hBMP;
		DWORD* pPixels;

		memset(&bmi, 0, sizeof(BITMAPINFO));
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		bmi.bmiHeader.biHeight = (LONG)vFinalTextureDims.y;
		bmi.bmiHeader.biWidth = (LONG)vFinalTextureDims.x;
		hBMP = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (void**)&pPixels, 0, 0);
		if (!hBMP)
		{
			SelectObject(hDC, oldFont);
			DeleteObject(font);
			DeleteDC(hDC);
			Log::getPointer()->exception("TextFontManager::buildFontFiles() failed upon call to Win32 API call Create DIBSection()");
		}
		// Draw chars into bitmap
		SelectObject(hDC, hBMP);
		for (int i = 0; i < 256; ++i)
		{
			char ch = (char)i;
			TextOut(hDC, int(vTLPospx[i].x - charDesc[i].fABCa), (int)vTLPospx[i].y, &ch, 1);
		}
		GdiFlush();

		// Create Image object from DIBsection ready to save to disk
		Image imgOut;
		imgOut.createBlank((unsigned int)vFinalTextureDims.x, (unsigned int)vFinalTextureDims.y, 4);

		// Compute filenames for each of the textures
		std::string strOutputNameBase = strOutputBaseName;
		transform(strOutputNameBase.begin(), strOutputNameBase.end(), strOutputNameBase.begin(), ::tolower);
		strOutputNameBase.append("_");
		strOutputNameBase.append(std::to_string(iFontHeight));

		std::string strImagename = strOutputNameBase;
		strImagename.append(".tga");

		Colouruc colTmp;
		unsigned int iIndex;
		for (int iX = 0; iX < (int)vFinalTextureDims.x; iX++)
		{
			for (int iY = 0; iY < (int)vFinalTextureDims.y; iY++)
			{
				iIndex = iX + (iY * int(vFinalTextureDims.x));
				colTmp.r = (unsigned char)pPixels[iIndex];
				imgOut.setPixel(iX, iY, colTmp.r, colTmp.r, colTmp.r, colTmp.r);
			}
		}
		imgOut.saveAsTGA(strImagename, true);	// Save out the final image texture

		// Compute texture coordinates for Normal
		Vector2 vTexSpan;
		vTexSpan.set(1.0f / float(vFinalTextureDims.x), 1.0f / float(vFinalTextureDims.y));
		for (int i = 0; i < 256; ++i)
		{
			charDesc[i].vTexMin.x = vTLPospx[i].x;// - charDesc[i].fABCa;
			charDesc[i].vTexMin.y = vTLPospx[i].y;

			charDesc[i].vTexMax.x = charDesc[i].vTexMin.x;
			charDesc[i].vTexMax.y = charDesc[i].vTexMin.y;
			charDesc[i].vTexMax.x += charDesc[i].fABCb;// +charDesc[i].fABCa;
			charDesc[i].vTexMax.y += fMaxHeight;// fCharHeight;

			// Invert and swap
			charDesc[i].vTexMin.y = vFinalTextureDims.y - charDesc[i].vTexMin.y;
			charDesc[i].vTexMax.y = vFinalTextureDims.y - charDesc[i].vTexMax.y;
			float fTmp = charDesc[i].vTexMin.y;
			charDesc[i].vTexMin.y = charDesc[i].vTexMax.y;
			charDesc[i].vTexMax.y = fTmp;

			// DEBUG
			//imgOut.setPixel((unsigned int)charDesc[i].vTexMin.x, (unsigned int)charDesc[i].vTexMin.y, Colour(255,0,0,255));
			//imgOut.setPixel((unsigned int)charDesc[i].vTexMax.x, (unsigned int)charDesc[i].vTexMax.y, Colour(255,255,0,255));

			// Convert to 0.0f to 1.0f range
			charDesc[i].vTexMax.x *= vTexSpan.x;
			charDesc[i].vTexMax.y *= vTexSpan.y;
			charDesc[i].vTexMin.x *= vTexSpan.x;
			charDesc[i].vTexMin.y *= vTexSpan.y;
		}

		// Save final font data file
		FILE* f;
		strOutputNameBase.append(".fnt");
		fopen_s(&f, strOutputNameBase.c_str(), "wb");
		if (!f)
			Log::getPointer()->exception("TextFontManager::buildFontFiles() failed. Unable to open file for saving.");

		// Write out max char height of each of the fonts
		if (1 != fwrite(&fMaxHeight, sizeof(float), 1, f))
			Log::getPointer()->exception("TextFontManager::buildFontFiles() failed. Error while saving.");

		// Write out each of the characters' info, for the normal font
		if (256 != fwrite(charDesc, sizeof(TextFont::CharDesc), 256, f))
		{
			fclose(f);
			if (!f)
				Log::getPointer()->exception("TextFontManager::buildFontFiles() failed. Error while saving.");
		}

		fclose(f);

		// Cleanup
		DeleteObject(hBMP);
		SelectObject(hDC, oldFont);	// Return old object
		DeleteObject(font);
		DeleteDC(hDC);
	}
}