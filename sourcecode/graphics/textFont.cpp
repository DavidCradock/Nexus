#include "precompiled_header.h"
#include "textFont.h"
#include "../managers/managerArchives.h"
#include "../managers/managerShaders.h"
#include "../managers/managerTextures.h"
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
		fontTypes.mstrTextureName = strFontTGAFilename;

		// Add textures to texture manager
		TextureManager* pTM = TextureManager::getPointer();
		pTM->add2DTexture(fontTypes.mstrTextureName, fontTypes.mstrTextureName, "fonts", false, TextureFiltering::nearest);

		// Load in font data
		ArchiveData archiveData;
		if (!archiveData.loadZipDisk(strFontFNTFilename))
		{
			std::string err("TextFont::load() failed. Unable to load font data file: ");
			err.append(strFontFNTFilename);
			throw std::runtime_error(err);
		}

		// Read in max char height of each of the fonts
		if (!archiveData.read(fontTypes.mfMaxCharHeight))
			throw std::runtime_error("TextFont::load() failed.");

		for (int i = 0; i < 256; ++i)
		{
			if (!archiveData.read(fontTypes.mCharDesc[i]))
				throw std::runtime_error("TextFont::load() failed. Error whilst loading in data from font data file");
		}

		bLoaded = true;
	}

	void TextFont::unload(void)
	{
		if (!bLoaded)
			return;

		TextureManager* pTM = TextureManager::getPointer();
		pTM->remove2DTexture(fontTypes.mstrTextureName, "fonts");
		bLoaded = false;
	}

	void TextFont::print(const std::string& strText, int iPosX, int iPosY, const CColouruc& colour)
	{
		if (!bLoaded)
			return;

		RenderDevice* pRD = RenderDevice::getPointer();
		TextureManager* pTM = TextureManager::getPointer();
		Texture* pTexture = pTM->get2DTexture(fontTypes.mstrTextureName, "fonts");
		pTexture->bind();
		Shader* pShader = ManagerShaders::getPointer()->getShader("textFont");
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
		if (!bLoaded)
			return 0;

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

}