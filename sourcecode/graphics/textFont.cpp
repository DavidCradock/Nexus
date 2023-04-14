#include "precompiled_header.h"
#include "textFont.h"
#include "texture.h"
#include "../archiveManager.h"
#include "../log.h"
#include "renderDevice.h"
#include "image.h"
#include "vertexBuffer.h"
#include "shader.h"

namespace Nexus
{

	TextFont::TextFont()
	{
	}

	TextFont::~TextFont()
	{
		unload();
		
	}

	void TextFont::load(const std::string& strFontFilePairName)
	{
		// Compute file pair filenames
		std::string strFontFNTFilename = strFontFilePairName;
		strFontFNTFilename.append(".fnt");
		std::string strFontTGAFilename = strFontFilePairName;
		strFontTGAFilename.append(".tga");
		fontTypes.mstrTextureName = strFontTGAFilename;

		// Add textures to texture manager
		TextureManager* pTM = TextureManager::getPointer();
		pTM->add2DTexture(fontTypes.mstrTextureName, fontTypes.mstrTextureName, "fonts", false);	// Should filter to nearest for perfect non-scaled text

		// Load in font data
		ArchiveData archiveData;
		if (!archiveData.loadZipDisk(strFontFNTFilename))
		{
			std::string strError("TextFont::load() failed. Unable to load font data file: ");
			strError.append(strFontFNTFilename);
			Log::getPointer()->addException(strError.c_str());
		}

		// Read in max char height of each of the fonts
		if (!archiveData.read(fontTypes.mfMaxCharHeight))
			Log::getPointer()->addException("TextFont::load() failed.");

		for (int i = 0; i < 256; ++i)
		{
			if (!archiveData.read(fontTypes.mCharDesc[i]))
				Log::getPointer()->addException("TextFont::load() failed. Error whilst loading in data from font data file");
		}

	}

	void TextFont::unload(void)
	{
		TextureManager* pTM = TextureManager::getPointer();
		pTM->remove2DTexture(fontTypes.mstrTextureName, "fonts");
	}

	void TextFont::print(const std::string& strText, int iPosX, int iPosY, const CColouruc& colour)
	{
		RenderDevice* pRD = RenderDevice::getPointer();
		TextureManager* pTM = TextureManager::getPointer();
		Texture* pTexture = pTM->get2DTexture(fontTypes.mstrTextureName, "fonts");
		pTexture->bind();
		Shader* pShader = ShaderManager::getPointer()->getShader("textFont");
		pShader->use();
		pShader->setInt("texture1", pTexture->getID());
		//glDisable(GL_BLEND);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		Vector2 vRTDims;
		vRTDims.x = (float)pRD->getWindowWidth();
		vRTDims.y = (float)pRD->getWindowHeight();
		
		Matrix matrixOrtho;
		matrixOrtho.setOrthographic();
		Matrix matrixTransform;
		matrixTransform.setIdentity();
		
		pShader->setMat4("transform", matrixOrtho * matrixTransform);
		
		// fABCa is the distance to add to the current position before drawing the character glyph.
		// fABCb is the width of the drawn portion of the character glyph. 
		// fABCc is the distance to add to the current position to provide white space to the right of the character glyph.

		// Build the vertex buffer for the given text
		char charIndex = 0;
		Vector2 vPosition((float)iPosX, (float)iPosY);
		Vector2 vDimensions;
		vDimensions.y = fontTypes.mfMaxCharHeight;
		Vector3 vColour(float(colour.r/255), float(colour.g / 255), float(colour.b / 255));
		Vector2 tcBL, tcBR, tcTR, tcTL;
		vertexBuffer.reset();
		for (unsigned int i = 0; i < strText.length(); ++i)
		{
			// Obtain character index
			const char* pCh = strText.c_str();
			charIndex = pCh[i];

			// Compute everything for this character 
			vPosition.x += fontTypes.mCharDesc[charIndex].fABCa;
			vDimensions.x = fontTypes.mCharDesc[charIndex].fABCb;
			
			tcBL.x = fontTypes.mCharDesc[charIndex].vTexMin.x;
			tcBL.y = fontTypes.mCharDesc[charIndex].vTexMin.y;
			tcTR.x = fontTypes.mCharDesc[charIndex].vTexMax.x;
			tcTR.y = fontTypes.mCharDesc[charIndex].vTexMax.y;
			tcBR.x = fontTypes.mCharDesc[charIndex].vTexMax.x;
			tcBR.y = fontTypes.mCharDesc[charIndex].vTexMin.y;
			tcTL.x = fontTypes.mCharDesc[charIndex].vTexMin.x;
			tcTL.y = fontTypes.mCharDesc[charIndex].vTexMax.y;

			// Add quad to vertex buffer
			vertexBuffer.addQuad(vPosition, vDimensions, vColour, tcBL, tcBR, tcTR, tcTL);

			vPosition.x += fontTypes.mCharDesc[charIndex].fABCb + fontTypes.mCharDesc[charIndex].fABCc;
		}
		// Then finally upload to the GPU and draw everything.
		vertexBuffer.upload();
		vertexBuffer.draw();
		
	}

	float TextFont::getTextWidth(const std::string& strText)
	{
		float fWidth = 0;
		unsigned char ch;
		for (int i = 0; i < (int)strText.length(); ++i)
		{
			ch = unsigned char(strText[i]);
			fWidth += fontTypes.mCharDesc[ch].fABCa;
			fWidth += fontTypes.mCharDesc[ch].fABCb;
			fWidth += fontTypes.mCharDesc[ch].fABCc;
		}
		return fWidth;
	}

	void TextFont::buildFontFiles(const std::string& strOutputBaseName, const std::string& strFontName, unsigned int iFontHeight, bool bAntialiased, bool bBold, bool bItalic, bool bUnderlined, bool bStrikeout)
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
			std::string str("TextRenderer::buildFontFiles() failed upon call to Win32 API call CreateFont()");
			Log::getPointer()->addException(str.c_str());
		}

		hDC = CreateCompatibleDC(0);
		if (0 == hDC)
		{
			DeleteObject(font);
			Log::getPointer()->addException("TextRenderer::buildFontFiles() failed upon call to Win32 API call CreateCompatibleDC()");
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

		TextFont::SCharDesc charDesc[256];


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
			Log::getPointer()->addException("TextRenderer::buildFontFiles() failed upon call to Win32 API call Create DIBSection()");
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

		CColouruc colTmp;
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
			Log::getPointer()->addException("TextRenderer::buildFontFiles() failed. Unable to open file for saving.");

		// Write out max char height of each of the fonts
		if (1 != fwrite(&fMaxHeight, sizeof(float), 1, f))
			Log::getPointer()->addException("TextRenderer::buildFontFiles() failed. Error while saving.");

		// Write out each of the characters' info, for the normal font
		if (256 != fwrite(charDesc, sizeof(TextFont::SCharDesc), 256, f))
		{
			fclose(f);
			if (!f)
				Log::getPointer()->addException("TextRenderer::buildFontFiles() failed. Error while saving.");
		}

		fclose(f);

		// Cleanup
		DeleteObject(hBMP);
		SelectObject(hDC, oldFont);	// Return old object
		DeleteObject(font);
		DeleteDC(hDC);
	}

}





/*  ALL COMMENTS BELOW!!!
*
	int CResFont::printex(const CText& strText, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwFormat, int iTextNumChars, const CColouruc& colour, bool bCalculateHeight)
	{
		/*
		if (!mFontNormal)
			CRITICAL_ERROR_CLASS("CResFont::printex() failed. The font isn't in the loaded state.");

		int iNumChars = iTextNumChars;
		if (-1 == iNumChars)
			iNumChars = strText.length();
		if (iNumChars > (int)strText.length())
			iNumChars = strText.length();
		RECT rect;
		rect.left = iPosX;
		rect.right = iPosX + iWidth;
		rect.top = iPosY;
		rect.bottom = iPosY + iHeight;

		DWORD format = dwFormat;
		if (bCalculateHeight)
			format |= DT_CALCRECT;

		return mFontNormal->DrawText(NULL, strText.c_str(), iTextNumChars, &rect, format, colour);
		return 0;
	}
	*/
	/*
		int CResFont::getMaxHeight(void)
		{
			/*
			if (!mFontNormal)
				CRITICAL_ERROR_CLASS("CResFont::getMaxHeight() failed. The font isn't in the loaded state.");

			TEXTMETRIC tm;
			mFontNormal->GetTextMetricsA(&tm);
			LONG maxHeight = tm.tmHeight;
			mFontBold->GetTextMetricsA(&tm);
			if (maxHeight < tm.tmHeight)
				maxHeight = tm.tmHeight;
			mFontItalic->GetTextMetricsA(&tm);
			if (maxHeight < tm.tmHeight)
				maxHeight = tm.tmHeight;

			return int(maxHeight);
			return 0;
		}
	*/

	/*
	int CResFont::printcodes(const CText& strText, int iPosX, int iPosY, int iWidth, int iHeight, const CColouruc& defaultColour, bool bComputeHeight)
	{

		if (!mFontNormal)
			CRITICAL_ERROR_CLASS("CResFont::printcodes() failed. The font isn't in the loaded state.");

		// Deal with extracting render commands from the text
		_createPrintCommands(strText, iPosX, iPosY, iWidth, iHeight, defaultColour);

		if (bComputeHeight)	// Just compute and return height
		{
			int iHeight = 0;
			if (_mvPC.size() == 0)
				return iHeight;
			iHeight = int(_mvPC[_mvPC.size() - 1].vPos.y) + getMaxHeight() - iPosX;
			return iHeight;
		}

		// Loop through each SPrintCommand and render them
		RECT rect;
		for (int i = 0; i < (int)_mvPC.size(); ++i)
		{
			// Compute bounds rect for this print command
			rect.left = (LONG)_mvPC[i].vPos.x;
			rect.top = (LONG)_mvPC[i].vPos.y;
			rect.right = LONG(iWidth + _mvPC[i].vPos.x);
			rect.bottom = LONG(iHeight + _mvPC[i].vPos.y);

			// Print text using correct type (normal, bold, italic)
			switch (_mvPC[i].iType)
			{
			case 0:
				mFontNormal->DrawText(NULL, _mvPC[i].txt.c_str(), _mvPC[i].txt.getLength(), &rect, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, _mvPC[i].col);
				break;
			case 1:
				mFontBold->DrawText(NULL, _mvPC[i].txt.c_str(), _mvPC[i].txt.getLength(), &rect, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, _mvPC[i].col);
				break;
			case 2:
				mFontItalic->DrawText(NULL, _mvPC[i].txt.c_str(), _mvPC[i].txt.getLength(), &rect, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, _mvPC[i].col);
				break;
			}
		}

		return 0;
	}
	*/

	/*
	CVector2 CResFont::getPrintcodesDims(const CText& strText)
	{
		CVector2 vDims;
		vDims.x = 0;
		vDims.y = 0;

//		if (!mFontNormal)
//			CRITICAL_ERROR_CLASS("CResFont::getPrintcodesDims() failed. The font isn't in the loaded state.");

		// Deal with extracting render commands from the text
		_createPrintCommands(strText, 0, 0, 4096, 4096, CColouruc());

		// Compute height
		int iHeight = 0;
		if (_mvPC.size() == 0)
			return vDims;
		iHeight = int(_mvPC[_mvPC.size() - 1].vPos.y) + getMaxHeight();
		vDims.y = float(iHeight);

		// Loop through each SPrintCommand
		RECT rect;
		for (int i = 0; i < (int)_mvPC.size(); ++i)
		{
			// Compute bounds rect for this print command
			rect.left = (LONG)_mvPC[i].vPos.x;
			rect.top = (LONG)_mvPC[i].vPos.y;
			rect.right = rect.left;
			rect.bottom = rect.top;

			switch (_mvPC[i].iType)
			{
			case 0:
//				mFontNormal->DrawText(NULL, _mvPC[i].txt.c_str(), _mvPC[i].txt.getLength(), &rect, DT_LEFT | DT_SINGLELINE | DT_NOCLIP | DT_CALCRECT, _mvPC[i].col);
				break;
			case 1:
//				mFontBold->DrawText(NULL, _mvPC[i].txt.c_str(), _mvPC[i].txt.getLength(), &rect, DT_LEFT | DT_SINGLELINE | DT_NOCLIP | DT_CALCRECT, _mvPC[i].col);
				break;
			case 2:
//				mFontItalic->DrawText(NULL, _mvPC[i].txt.c_str(), _mvPC[i].txt.getLength(), &rect, DT_LEFT | DT_SINGLELINE | DT_NOCLIP | DT_CALCRECT, _mvPC[i].col);
				break;
			}
			if (rect.right > (int)vDims.x)
				vDims.x = float(rect.right);
		}

		//	vDims.x += 10;
		//	vDims.y += 10;

		return vDims;
	}
	*/

	/*
	int CResFont::_extractColCode(CText& strText, CColouruc& col)
	{
		// Try to find a position of a colour code
		int iPos = strText.findSubtext("[!col");
		if (iPos != -1)	// Found one! (Possibly, it might be invalid)
		{
			// Based on position of the found "[col" substring, see if we have a valid colour code...
			CText strCol;	// Will hold colour as hexidecimal, IE "FFFFFFFF"
			// Make sure that there's enough chars to even hold a [!colffffffff] code
			if (iPos + 13 > (int)strText.getLength())
				return -1;	// Not enough room to even store the full colour code
			if (strText.c_str()[iPos + 13] != ']')	// Code doesn't end in expected ']'
				return -1;
			// Copy colour to strCol
			for (int i = iPos + 5; i < iPos + 13; ++i)
			{
				char ch[2];
				ch[1] = 0;
				ch[0] = strText.c_str()[i];
				strCol.append(ch);
			}

			CColouruc colTmp;
			if (!strCol.convertHexToCol(colTmp))	// Isn't a valid hex value
				return -1;
			else
			{
				col = colTmp;
				return iPos;
			}
		}
		return -1;
	}
	*/

	/*
		void CResFont::_createPrintCommands(const CText& strText, int iPosX, int iPosY, int iWidth, int iHeight, const CColouruc& defaultColour)
		{
			// This will hold print commands
			std::vector<SPrintCommand> vPCTmp;

			// Make working copy of parsed text string
			CText txtTmp = strText;

			// Convert all "\n" to "[!nl]"
			txtTmp.replaceAllSubtext("\n", "[!nl]");

			// Insert default text settings at front of text
			CText strDefaultColAsHex;	strDefaultColAsHex.convertColToHex(defaultColour);
			CText txtDefaultCodes("[!n][!col");	txtDefaultCodes.append(strDefaultColAsHex);
			txtDefaultCodes.append("]");
			txtTmp.insertChars(0, txtDefaultCodes);

			// Add a code (doesn't matter what type) to the end
			// This is to reduce additional testing in the while loop below...
			txtTmp.append("[!n]");

			SPrintCommand pcCur;	// Current font rendering settings
			pcCur.col = defaultColour;
			//	pcCur.vPos.set(float(iPosX), float(iPosY));
			pcCur.iType = 0;

			// Find first code position and store the code in strCode
			CText strCode;
			int iCodePos = _findFirstCode(txtTmp, strCode);
			while (iCodePos != -1)
			{
				// Extract code and adjust current rendering settings
				if (-1 != strCode.findSubtext("[!n]"))
				{
					// Change font type
					pcCur.iType = 0;
					pcCur.bNewline = false;	// Not a forced new line

					// Remove code from text
					txtTmp.removeChars(0, 4);

					// Find position of next code in text (Ready for next loop)
					iCodePos = _findFirstCode(txtTmp, strCode);

					// If the next code isn't at position zero, then there's normal text between now and the the next
					if (iCodePos != 0)
					{
						// Add text upto point of next code to the render command
						pcCur.txt.clear();
						pcCur.txt.insertChars(0, txtTmp, iCodePos);

						// Push the render command onto the array ready for rendering
						vPCTmp.push_back(pcCur);

						// Remove from text string we just pushed
						txtTmp.removeChars(0, iCodePos);

						// Find position of next code in text (Ready for next loop)
						iCodePos = _findFirstCode(txtTmp, strCode);
					}
				}
				else if (-1 != strCode.findSubtext("[!b]"))
				{
					// Change font type
					pcCur.iType = 1;
					pcCur.bNewline = false;	// Not a forced new line

					// Remove code from text
					txtTmp.removeChars(0, 4);

					// Find position of next code in text (Ready for next loop)
					iCodePos = _findFirstCode(txtTmp, strCode);

					// If the next code isn't at position zero, then there's normal text between now and the the next
					if (iCodePos != 0)
					{
						// Add text upto point of next code to the render command
						pcCur.txt.clear();
						pcCur.txt.insertChars(0, txtTmp, iCodePos);

						// Push the render command onto the array ready for rendering
						vPCTmp.push_back(pcCur);

						// Remove from text string we just pushed
						txtTmp.removeChars(0, iCodePos);

						// Find position of next code in text (Ready for next loop)
						iCodePos = _findFirstCode(txtTmp, strCode);
					}
				}
				else if (-1 != strCode.findSubtext("[!i]"))
				{
					// Change font type
					pcCur.iType = 2;
					pcCur.bNewline = false;	// Not a forced new line

					// Remove code from text
					txtTmp.removeChars(0, 4);

					// Find position of next code in text (Ready for next loop)
					iCodePos = _findFirstCode(txtTmp, strCode);

					// If the next code isn't at position zero, then there's normal text between now and the the next
					if (iCodePos != 0)
					{
						// Add text upto point of next code to the render command
						pcCur.txt.clear();
						pcCur.txt.insertChars(0, txtTmp, iCodePos);

						// Push the render command onto the array ready for rendering
						vPCTmp.push_back(pcCur);

						// Remove from text string we just pushed
						txtTmp.removeChars(0, iCodePos);

						// Find position of next code in text (Ready for next loop)
						iCodePos = _findFirstCode(txtTmp, strCode);
					}
				}
				else if (-1 != _extractColCode(strCode, pcCur.col))
				{
					// Colour already stored in pCur.col
					pcCur.bNewline = false;	// Not a forced new line

					// Remove code from text ([!colffffffff])
					txtTmp.removeChars(0, 14);

					// Find position of next code in text (Ready for next loop)
					iCodePos = _findFirstCode(txtTmp, strCode);

					// If the next code isn't at position zero, then there's normal text between now and the the next
					if (iCodePos != 0)
					{
						// Add text upto point of next code to the render command
						pcCur.txt.clear();
						pcCur.txt.insertChars(0, txtTmp, iCodePos);

						// Push the render command onto the array ready for rendering
						vPCTmp.push_back(pcCur);

						// Remove from text string we just pushed
						txtTmp.removeChars(0, iCodePos);

						// Find position of next code in text (Ready for next loop)
						iCodePos = _findFirstCode(txtTmp, strCode);
					}

				}
				else if (-1 != strCode.findSubtext("[!nl]"))
				{
					pcCur.bNewline = true;	// Forced new line

					// Remove code from text
					txtTmp.removeChars(0, 5);

					// Find position of next code in text (Ready for next loop)
					iCodePos = _findFirstCode(txtTmp, strCode);

					// If the next code isn't at position zero, then there's normal text between now and the the next
					if (iCodePos != 0)
					{
						// Add text upto point of next code to the render command
						pcCur.txt.clear();
						pcCur.txt.insertChars(0, txtTmp, iCodePos);

						// Push the render command onto the array ready for rendering
						vPCTmp.push_back(pcCur);

						// Remove from text string we just pushed
						txtTmp.removeChars(0, iCodePos);

						// Find position of next code in text (Ready for next loop)
						iCodePos = _findFirstCode(txtTmp, strCode);
					}
				}
			}

			// Copy vPCTmp containing the print commands to _mvPC, computing positions and splitting if required as we go
			_mvPC.clear();
			CVector2 vCurPos;
			vCurPos.set(float(iPosX), float(iPosY));					// Holds current position
			float fRightEdge = float(iPosX) + float(iWidth);			// Right edge bounding rect pos
			float fLineHeight = (float)getMaxHeight();							// Amount to offset Y pos when newline needed
			std::vector<SPrintCommand>::iterator it = vPCTmp.begin();	// Loop through each current render command
			while (it != vPCTmp.end())
			{
				if (it->bNewline)
				{
					vCurPos.x = float(iPosX);
					vCurPos.y += fLineHeight;
				}

				float fTextWidth = _getTextWidth(it->txt, it->iType);
				if (vCurPos.x + fTextWidth < fRightEdge)	// If the render command fits within rect bounds
				{
					it->vPos = vCurPos;
					_mvPC.push_back((SPrintCommand)*it);
					vCurPos.x += fTextWidth;
				}
				else	// Doesn't fit, gonna have to split the thing
				{
					SPrintCommand pcNew;

					pcNew.col = it->col;
					pcNew.iType = it->iType;
					pcNew.vPos = vCurPos;

					CText strWord;
					float fWordWidth;
					// While there are still characters/words in the print command
					while (it->txt.getLength() > 0)
					{
						fTextWidth = _getTextWidth(pcNew.txt, pcNew.iType);

						it->txt.extractFirstWord(strWord);
						// Add the word back
						it->txt.insertChars(0, strWord, -1);

						fWordWidth = _getTextWidth(strWord, pcNew.iType);

						if (fWordWidth + fTextWidth + pcNew.vPos.x >= fRightEdge)	// New word, when added, wouldn't fit
						{
							// But if there are no characters in the new pc, that's because the first word is just too big, add it anyway...
							if (pcNew.txt.getLength() == 0)
							{
								it->txt.removeChars(0, strWord.getLength());
								pcNew.txt.append(strWord);
							}

							_mvPC.push_back(pcNew);
							pcNew.txt.clear();
							vCurPos.x = float(iPosX);
							vCurPos.y += fLineHeight;
							pcNew.vPos = vCurPos;
						}
						else	// The word added, will fit
						{
							pcNew.txt.append(strWord);
							it->txt.removeChars(0, strWord.getLength());
						}
					}
					// Now we've run out of words, we may still have some text to render...
					if (pcNew.txt.getLength() != 0)
					{
						if (it->bNewline)
						{
							pcNew.vPos.x = float(iPosX);
							pcNew.vPos.y = vCurPos.y;
							vCurPos.y += fLineHeight;
						}
						_mvPC.push_back(pcNew);
					}
				}	// Doesn't fit, gonna have to split the thing
				it++;	// Next print code
			}	// while (it != vPCTmp.end())

		}
		*/

		/*
		// Finds the next complete code in the given strText, stores it in strCode and returns it's position
		// Returns -1 if no command found in strText
		int CResFont::_findFirstCode(CText& strText, CText& strCode)
		{
			int iCodePos_NL = strText.findSubtext("[!nl]");
			int iCodePos_B = strText.findSubtext("[!b]");
			int iCodePos_N = strText.findSubtext("[!n]");
			int iCodePos_I = strText.findSubtext("[!i]");
			CColouruc col;
			int iCodePos_Col = _extractColCode(strText, col);

			int iStrLen = (int)strText.getLength();

			int iFirstResPos = iStrLen;
			if (iCodePos_NL != -1)
			{
				if (iCodePos_NL < iFirstResPos)
				{
					iFirstResPos = iCodePos_NL;
					strCode = "[!nl]";
				}
			}
			if (iCodePos_B != -1)
			{
				if (iCodePos_B < iFirstResPos)
				{
					iFirstResPos = iCodePos_B;
					strCode = "[!b]";
				}
			}
			if (iCodePos_N != -1)
			{
				if (iCodePos_N < iFirstResPos)
				{
					iFirstResPos = iCodePos_N;
					strCode = "[!n]";
				}
			}
			if (iCodePos_I != -1)
			{
				if (iCodePos_I < iFirstResPos)
				{
					iFirstResPos = iCodePos_I;
					strCode = "[!i]";
				}
			}
			if (iCodePos_Col != -1)
			{
				if (iCodePos_Col < iFirstResPos)
				{
					iFirstResPos = iCodePos_Col;
					strCode.clear();
					for (int i = 0; i < 14; ++i)
					{
						char ch[2];
						ch[1] = 0;
						ch[0] = strText.c_str()[iCodePos_Col + i];
						strCode.append(ch);
					}
				}
			}

			if (iFirstResPos == iStrLen)
				return -1;
			return iFirstResPos;
		}

		*/

		/*
		float CResFont::_getTextWidth(const CText& txt, int iType)
		{
	//		if (!mFontNormal)
	//			CRITICAL_ERROR_CLASS("CResFont::_getTextWidth() failed. The font isn't in the loaded state.");
	//		if (iType < 0 || iType > 2)
	//			CRITICAL_ERROR_CLASS("CResFont::_getTextWidth() failed. Invalid iType value given.");

	//		D3DCOLOR col = D3DCOLOR_RGBA(255, 255, 255, 255);
			RECT rct;
			rct.left = 0;
			rct.right = 10000;
			rct.top = 0;
			rct.bottom = 100;
			// Add a '.' to end of txt to make sure DirectX takes into consideration any spaces
			CText txtTmp = txt;
			if (txtTmp.c_str()[txtTmp.size() - 1] == ' ')
				txtTmp.append(".");

			float fWidth = 0.0f;

			switch (iType)
			{
			case 0:
	//			mFontNormal->DrawText(NULL, txtTmp.c_str(), -1, &rct, DT_CALCRECT | DT_LEFT | DT_SINGLELINE | DT_NOCLIP, col);
				break;
			case 1:
	//			mFontBold->DrawText(NULL, txtTmp.c_str(), -1, &rct, DT_CALCRECT | DT_LEFT | DT_SINGLELINE | DT_NOCLIP, col);
				break;
			case 2:
	//			mFontItalic->DrawText(NULL, txtTmp.c_str(), -1, &rct, DT_CALCRECT | DT_LEFT | DT_SINGLELINE | DT_NOCLIP, col);
				break;
			};

			fWidth = float(rct.right - rct.left);
			return fWidth;
		}
		*/