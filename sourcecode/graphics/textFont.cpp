#include "precompiled_header.h"
#include "textFont.h"
#include "../managers/managers.h"
#include "../core/log.h"
#include "renderDevice.h"
#include "image.h"
#include "vertexBuffer.h"

namespace Nexus
{
	TextFont::TextFont()
	{
		bLoaded = false;
		refCount = 0;
	}

	TextFont::~TextFont()
	{
		unload();
	}

	void TextFont::load(const std::string& strFontFilePairName)
	{
		if (bLoaded)
			return;

		// Compute file pair filenames
		std::string strFontFNTFilename = strFontFilePairName;
		strFontFNTFilename.append(".fnt");
		std::string strFontTGAFilename = strFontFilePairName;
		strFontTGAFilename.append(".tga");
		fontTypes.strTextureName = strFontTGAFilename;

		// Add textures to texture manager
		Managers* pMan = Managers::getPointer();
		pMan->textures->add(fontTypes.strTextureName, fontTypes.strTextureName, "fonts", false, TextureFiltering::nearest);

		// Load in font data
		ArchiveData archiveData;
		if (!archiveData.loadZipDisk(strFontFNTFilename))
		{
			std::string err("TextFont::load() failed. Unable to load font data file: ");
			err.append(strFontFNTFilename);
			Log::getPointer()->exception(err);
		}

		// Read in max char height of each of the fonts
		if (!archiveData.read(fontTypes.fMaxCharHeight))
			Log::getPointer()->exception("TextFont::load() failed.");

		for (int i = 0; i < 256; ++i)
		{
			if (!archiveData.read(fontTypes.charDesc[i]))
				Log::getPointer()->exception("TextFont::load() failed. Error whilst loading in data from font data file");
		}

		bLoaded = true;
	}

	void TextFont::unload(void)
	{
		if (!bLoaded)
			return;

		Managers* pMan = Managers::getPointer();
		pMan->textures->remove(fontTypes.strTextureName, "fonts");
		bLoaded = false;
	}

	void TextFont::print(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, const Colouruc& colour)
	{
		if (!bLoaded)
			return;

		RenderDevice* pRD = RenderDevice::getPointer();
		Managers* pMan = Managers::getPointer();
		Texture* pTexture = pMan->textures->get(fontTypes.strTextureName, "fonts");
		pTexture->bind();
		Shader* pShader = pMan->shaders->get("default");
		pShader->bind();
		pShader->setInt("texture1", pTexture->getID());
		//glDisable(GL_BLEND);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		Vector2 vRTDims;
		vRTDims.x = (float)iRenderTargetWidth;
		vRTDims.y = (float)iRenderTargetHeight;
		
		Matrix matrixOrtho;
		matrixOrtho.setOrthographic(0.0f, vRTDims.x, 0.0f, vRTDims.y);
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
		vDimensions.y = fontTypes.fMaxCharHeight;
		Vector4 vColour(float(colour.r/255.0f), float(colour.g / 255.0f), float(colour.b / 255.0f), float(colour.a / 255.0f));
		Vector2 tcBL, tcBR, tcTR, tcTL;
		vertexBuffer.reset();
		for (unsigned int i = 0; i < strText.length(); ++i)
		{
			// Obtain character index
			const char* pCh = strText.c_str();
			charIndex = pCh[i];

			// Compute everything for this character 
			vPosition.x += fontTypes.charDesc[charIndex].fABCa;
			vDimensions.x = fontTypes.charDesc[charIndex].fABCb;
			
			tcBL.x = fontTypes.charDesc[charIndex].vTexMin.x;
			tcBL.y = fontTypes.charDesc[charIndex].vTexMin.y;
			tcTR.x = fontTypes.charDesc[charIndex].vTexMax.x;
			tcTR.y = fontTypes.charDesc[charIndex].vTexMax.y;
			tcBR.x = fontTypes.charDesc[charIndex].vTexMax.x;
			tcBR.y = fontTypes.charDesc[charIndex].vTexMin.y;
			tcTL.x = fontTypes.charDesc[charIndex].vTexMin.x;
			tcTL.y = fontTypes.charDesc[charIndex].vTexMax.y;

			// Add quad to vertex buffer
			vertexBuffer.addQuad(vPosition, vDimensions, vColour, tcBL, tcBR, tcTR, tcTL);

			vPosition.x += fontTypes.charDesc[charIndex].fABCb + fontTypes.charDesc[charIndex].fABCc;
		}
		// Then finally upload to the GPU and draw everything.
		vertexBuffer.upload();
		vertexBuffer.draw();
		
	}

	void TextFont::print(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, const Colourf& colour)
	{
		Colouruc col;
		col.setf(colour.r, colour.g, colour.b, colour.a);
		print(strText, iPosX, iPosY, iRenderTargetWidth, iRenderTargetHeight, col);
	}

	void TextFont::printCentered(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, const Colouruc& colour)
	{
		float fTextWidth = getTextWidth(strText);
		iPosX -= int(fTextWidth * 0.5f);
		iPosY -= int(getTextHeight() * 0.5f);
		print(strText, iPosX, iPosY, iRenderTargetWidth, iRenderTargetHeight, colour);
	}

	void TextFont::printCentered(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, const Colourf& colour)
	{
		Colouruc col;
		col.setf(colour.r, colour.g, colour.b, colour.a);
		float fTextWidth = getTextWidth(strText);
		iPosX -= int(fTextWidth * 0.5f);
		iPosY -= int(getTextHeight() * 0.5f);
		print(strText, iPosX, iPosY, iRenderTargetWidth, iRenderTargetHeight, col);

	}

	float TextFont::getTextWidth(const std::string& strText)
	{
		if (!bLoaded)
			return 0;

		float fWidth = 0;
		unsigned char ch;
		for (int i = 0; i < (int)strText.length(); ++i)
		{
			ch = unsigned char(strText[i]);
			fWidth += fontTypes.charDesc[ch].fABCa;
			fWidth += fontTypes.charDesc[ch].fABCb;
			fWidth += fontTypes.charDesc[ch].fABCc;
		}
		return fWidth;
	}
}