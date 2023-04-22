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

		mvFrameTextures.push_back(strTextureFilename);
		if (fTimeToShowThisFrameMS < 1.0f)
			fTimeToShowThisFrameMS = 1.0f;
		mvfFrameDelay.push_back(fTimeToShowThisFrameMS);
		pManTextures->add2DTexture(strTextureFilename, strTextureFilename, "sprites", true);
		mvNonScaledDims.setZero();	// So it's re-calculated (In case the diffuse texture has changed
	}

	void SpriteDescription::removeAllFrames(void)
	{
		ManagerTextures* pManTextures = ManagerTextures::getPointer();
		for (int i = 0; i < (int)mvFrameTextures.size(); ++i)
		{
			pManTextures->remove2DTexture(mvFrameTextures[i], "sprites");
		}
		for (int i = 0; i < (int)mvFrameTextures.size(); ++i)
		{
			mvfFrameDelay.pop_back();
			mvFrameTextures.pop_back();
		}

	}

	const std::string& SpriteDescription::getFrameTextureName(unsigned int iFrameNumber)
	{
		if (iFrameNumber >= (unsigned int)mvFrameTextures.size())
		{
			std::string err("SpriteDescription::getFrameTextureName(");
			err.append(std::to_string(iFrameNumber));
			err.append(") failed. Invalid iFrameNumber given. Must be at most x-1 where x is the total number of frames, which in this case is ");
			err.append(std::to_string(mvFrameTextures.size()));
			Log::getPointer()->exception(err);
		}
		return mvFrameTextures[iFrameNumber];
	}

	float SpriteDescription::getFrameDelayMS(unsigned int iFrameNumber)
	{
		if (iFrameNumber >= (unsigned int)mvFrameTextures.size())
		{
			std::string err("SpriteDescription::getFrameDelayMS(");
			err.append(std::to_string(iFrameNumber));
			err.append(") failed. Invalid iFrameNumber given. Must be at most x-1 where x is the total number of frames, which in this case is ");
			err.append(std::to_string(mvFrameTextures.size()));
			Log::getPointer()->exception(err);
		}
		return mvfFrameDelay[iFrameNumber];
	}

}