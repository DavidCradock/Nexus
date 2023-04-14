#pragma once
#include "../../precompiled_header.h"
#include "../../math/vector2.h"

namespace Nexus
{

	// This holds the description of a sprite.
	// These are created to represent the appearance, shape, collision of a sprite.
	// Sprite entities use these to draw themselves.
	// You create these from the SpriteManager class found in spriteManager.h
	class SpriteDescription
	{
		friend class SpriteLayer;
		friend class SpriteEntity;
	public:
		~SpriteDescription();

		// Adds a texture filename to use for an animation frame.
		// Can be only one, or multiple calls can be called to add additional animation frames.
		// Also sets mvNonScaledDims to zero (so it's calculated again)
		// Also adds the texture to the resource group "sprites"
		// If called more than once, fTimeToShowThisFrameMS is used for the delay for the added frame.
		// For example, if you wish the animation to run at 30fps, then you'd put in here 1000.0f / 30.0f = 33.33333 milliseconds
		void addFrame(const std::string& strTextureFilename, float fTimeToShowThisFrameMS = 33.33333333f);

		// Removes all frames from this object.
		// Also removes/reduces reference count in the texture manager
		void removeAllFrames(void);

		// Returns the name of an animation frame's texture name
		// If invalid frame number is given, an exception occurs
		const std::string& getFrameTextureName(unsigned int iFrameNumber);

		// Returns the total number of animation frames added to this sprite desc
		unsigned int getNumFrames(void) { return unsigned int(mvFrameTextures.size()); }

		// Returns the frame number's delay time in milliseconds (for animation)
		// If invalid frame number is given, an exception occurs
		float getFrameDelayMS(unsigned int iFrameNumber);
	private:
		std::vector<std::string> mvFrameTextures;	// Name of all the textures that the sprite will use to render itself. May only be one, in which case it's a static sprite instead of animated.
		std::vector<float> mvfFrameDelay;	// Delay of each frame, in milliseconds
		Vector2 mvNonScaledDims;		// This is the width/height of the diffuse texture and therefore the non-scaled dimensions of a sprite using this description
		float mfBoundingCircleRadius;	// The bounding circle radius (computed along with mvNonScaledDims in C2DSpriteLayer::_renderEntities
	};

}

