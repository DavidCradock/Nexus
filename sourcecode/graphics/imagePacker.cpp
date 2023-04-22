#include "precompiled_header.h"
#include "imagePacker.h"
#include "../core/log.h"

namespace Nexus
{

	ImagePacker::ImagePacker()
	{

	}

	ImagePacker::~ImagePacker()
	{
		reset();
	}

	bool ImagePacker::addImage(const Image& newImage, const std::string& strID)
	{
		// Attempt to find named image
		std::map<std::string, ImagePackerImg*>::iterator it = addedImages.find(strID);
		if (it != addedImages.end())	// Already added
			return false;

		ImagePackerImg* pNew = new ImagePackerImg;
		if (!pNew)
		{
			std::string err("ImagePacker::addImage(const Image &newImage, \"");
			err.append(strID);
			err.append("\") failed. Unable to allocate memory.");
			Log::getPointer()->exception(err);
		}
		pNew->pImage = new Image;
		if (!pNew->pImage)
		{
			std::string err("ImagePacker::addImage(const Image &newImage, \"");
			err.append(strID);
			err.append("\") failed. Unable to allocate memory.");
			Log::getPointer()->exception(err);
		}
		addedImages[strID] = pNew;

		newImage.copyTo(*pNew->pImage);
		return true;
	}

	void ImagePacker::pack(bool bAllowRotation)
	{
		// Make sure we have got some images to work with 
		if (addedImages.size() < 1)
			return;

		// Clear arrays from map for images and ID so we can sort
		ID.clear();
		image.clear();
		rotated.clear();

		// Copy data into temporary arrays
		std::map<std::string, ImagePackerImg*>::iterator it = addedImages.begin();
		bool bRotated = false;
		while (it != addedImages.end())
		{
			// Rotate image so that it's height is greatest
			if (it->second->pImage->getWidth() > it->second->pImage->getHeight())
			{
				bRotated = true;
				it->second->pImage->rotateClockwise();
			}
			else
				bRotated = false;
			rotated.push_back(bRotated);
			image.push_back(it->second->pImage);
			ID.push_back(it->first);
			it++;
		}

		// Bubble sort temp arrays by their image's width
		Image* pImageTemp;
		std::string strIDTemp;
		for (int i = 0; i < (int)image.size(); ++i)
		{
			for (int j = 0; j < (int)image.size(); ++j)
			{
				// Compute area of both images
				// Swap by largest first
				if (image[j]->getWidth() < image[i]->getWidth())
				{
					pImageTemp = image[i];
					strIDTemp = ID[i];
					image[i] = image[j];
					ID[i] = ID[j];
					image[j] = pImageTemp;
					ID[j] = strIDTemp;
				}
			}
		}
		// Bubble sort temp arrays by their image's height
		for (int i = 0; i < (int)image.size(); ++i)
		{
			for (int j = 0; j < (int)image.size(); ++j)
			{
				// Compute area of both images
				// Swap by largest first
				if (image[j]->getHeight() < image[i]->getHeight())
				{
					pImageTemp = image[i];
					strIDTemp = ID[i];
					image[i] = image[j];
					ID[i] = ID[j];
					image[j] = pImageTemp;
					ID[j] = strIDTemp;
				}
			}
		}
		// Now, vID[] and vImg[] contains the ID and pointers to the images, sorted by greatest height first
		// We have to add them one at a time, to the final image, then store their texture coordinates and bottom left positions inside the map

		// Compute size of final image
		unsigned int iTotalSurfaceArea = 0;
		for (int i = 0; i < (int)image.size(); ++i)
		{
			iTotalSurfaceArea += image[i]->getWidth() * image[i]->getHeight();
		}
		iTotalSurfaceArea = unsigned int(float(iTotalSurfaceArea) * 1.3f);	// Adjust this for more efficiency of final image size, maybe 22% wastage?
		iTotalSurfaceArea = (unsigned int)sqrtf((float)iTotalSurfaceArea);
		unsigned int iFinalDims = 2;
		while (iFinalDims < iTotalSurfaceArea)
			iFinalDims *= 2;

		// Create final image
		while (!performPack(iFinalDims))
		{
			iFinalDims *= 2;
		}
	}

	bool ImagePacker::performPack(unsigned int iFinalImageDims)
	{
		imageFinal.createBlank(iFinalImageDims, iFinalImageDims, 4);

		// Insert first image, into bottom left of output image
		packInsertImg(image[0], 0, 0, 0);
		unsigned int iXpos = image[0]->getWidth();
		unsigned int iYpos = 0;
		unsigned int iFirstColHeight = image[0]->getHeight();
		for (int iImg = 1; iImg < (int)image.size(); ++iImg)
		{
			// If it fits 
			if (iXpos + image[iImg]->getWidth() <= imageFinal.getWidth())
			{
				packInsertImg(image[iImg], iImg, iXpos, iYpos);
				iXpos += image[iImg]->getWidth();
			}
			// Doesn't, start new row
			else
			{
				iXpos = 0;
				iYpos += iFirstColHeight;
				iFirstColHeight = image[iImg]->getHeight();

				// Check to see if the image will fit here, if it doesn't, we've screwed up with the calculation of the 
				// final texture size, so we're going to have to increase the final image size and start again :/
				if (iXpos + image[iImg]->getWidth() > imageFinal.getWidth())
					return false;
				if (iYpos + image[iImg]->getHeight() > imageFinal.getHeight())
					return false;

				packInsertImg(image[iImg], iImg, iXpos, iYpos);
				iXpos += image[iImg]->getWidth();
			}

		}
		// If we get here, everything went well, yay!
		return true;
	}

	void ImagePacker::packInsertImg(Image* pImageSrc, unsigned int iIndex, int iPosX, int iPosY)
	{
		// Copy the image into final
		pImageSrc->copyRectTo(imageFinal, 0, 0, pImageSrc->getWidth(), pImageSrc->getHeight(), iPosX, iPosY);

		// Compute and store position and texture coordinates
		float fOneOverWidth = 1.0f / float(imageFinal.getWidth());
		float fOneOverHeight = 1.0f / float(imageFinal.getHeight());

		std::map<std::string, ImagePackerImg*>::iterator it = addedImages.find(ID[iIndex]);
		if (it == addedImages.end())
			Log::getPointer()->exception("ImagePacker::packInsertImg() Critical error (this should never happen)");
		it->second->position.set(float(iPosX), float(iPosY));

		// Compute texture coordinates
		float ftcl = float(iPosX) * fOneOverWidth;
		float ftcr = ftcl + (float(pImageSrc->getWidth()) * fOneOverWidth);
		float ftcb = float(iPosY) * fOneOverHeight;
		float ftct = ftcb + (float(pImageSrc->getHeight()) * fOneOverHeight);
		if (!rotated[iIndex])	// Not rotated clockwise?
		{
			it->second->TCBL.set(ftcl, ftcb);
			it->second->TCBR.set(ftcr, ftcb);
			it->second->TCTR.set(ftcr, ftct);
			it->second->TCTL.set(ftcl, ftct);
		}
		else	// Rotated
		{
			it->second->TCBL.set(ftcl, ftct);
			it->second->TCBR.set(ftcl, ftcb);
			it->second->TCTR.set(ftcr, ftcb);
			it->second->TCTL.set(ftcr, ftct);
		}
	}

	//! Resets everything so we can start over
	void ImagePacker::reset(void)
	{
		std::map<std::string, ImagePackerImg*>::iterator it = addedImages.begin();
		while (it != addedImages.end())
		{
			delete it->second->pImage;
			delete it->second;
			it++;
		}
		addedImages.clear();
		imageFinal.free();
		ID.clear();
		image.clear();
		rotated.clear();
	}

	Image& ImagePacker::getFinalImage(void)
	{
		return imageFinal;
	}

	Vector2 ImagePacker::getImagePosition(const std::string& strID)
	{
		Vector2 vResult;

		// Attempt to find named image
		std::map<std::string, ImagePackerImg*>::iterator it = addedImages.find(strID);
		if (it == addedImages.end())	// Couldn't be found
			return vResult;
		return it->second->position;
	}

	void ImagePacker::getImageTextureCoords(const std::string& strID, Vector2& vTCBL, Vector2& vTCBR, Vector2& vTCTR, Vector2& vTCTL)
	{
		// Attempt to find named image
		std::map<std::string, ImagePackerImg*>::iterator it = addedImages.find(strID);
		if (it == addedImages.end())	// Couldn't be found
			return;
		vTCBL = it->second->TCBL;
		vTCBR = it->second->TCBR;
		vTCTR = it->second->TCTR;
		vTCTL = it->second->TCTL;
	}

}