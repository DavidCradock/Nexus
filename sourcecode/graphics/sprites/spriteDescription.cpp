#include "precompiled_header.h"
#include "spriteDescription.h"
#include "../../core/log.h"
#include "../../managers/managerTextures.h"

namespace Nexus
{

	SpriteDescription::~SpriteDescription()
	{
		removeAllFrames();
	}

	void SpriteDescription::addFrame(const std::string& strTextureFilename, float fTimeToShowThisFrameMS)
	{
		ManagerTextures* pManTextures = ManagerTextures::getPointer();

		vstrFrameTextures.push_back(strTextureFilename);
		if (fTimeToShowThisFrameMS < 1.0f)
			fTimeToShowThisFrameMS = 1.0f;
		vfFrameDelay.push_back(fTimeToShowThisFrameMS);
		pManTextures->add2DTexture(strTextureFilename, strTextureFilename, "sprites", true);
		nonScaledDims.setZero();	// So it's re-calculated (In case the diffuse texture has changed
	}

	void SpriteDescription::removeAllFrames(void)
	{
		ManagerTextures* pManTextures = ManagerTextures::getPointer();
		for (int i = 0; i < (int)vstrFrameTextures.size(); ++i)
		{
			pManTextures->remove2DTexture(vstrFrameTextures[i], "sprites");
		}
		for (int i = 0; i < (int)vstrFrameTextures.size(); ++i)
		{
			vfFrameDelay.pop_back();
			vstrFrameTextures.pop_back();
		}

	}

	const std::string& SpriteDescription::getFrameTextureName(unsigned int iFrameNumber)
	{
		if (iFrameNumber >= (unsigned int)vstrFrameTextures.size())
		{
			std::string err("SpriteDescription::getFrameTextureName(");
			err.append(std::to_string(iFrameNumber));
			err.append(") failed. Invalid iFrameNumber given. Must be at most x-1 where x is the total number of frames, which in this case is ");
			err.append(std::to_string(vstrFrameTextures.size()));
			Log::getPointer()->exception(err);
		}
		return vstrFrameTextures[iFrameNumber];
	}

	float SpriteDescription::getFrameDelayMS(unsigned int iFrameNumber)
	{
		if (iFrameNumber >= (unsigned int)vstrFrameTextures.size())
		{
			std::string err("SpriteDescription::getFrameDelayMS(");
			err.append(std::to_string(iFrameNumber));
			err.append(") failed. Invalid iFrameNumber given. Must be at most x-1 where x is the total number of frames, which in this case is ");
			err.append(std::to_string(vstrFrameTextures.size()));
			Log::getPointer()->exception(err);
		}
		return vfFrameDelay[iFrameNumber];
	}

}