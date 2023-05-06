#include "precompiled_header.h"
#include "sprites.h"
#include "../../managers/ShaderManager.h"
#include "../../managers/TextureManager.h"
#include "../../core/log.h"

namespace Nexus
{
	Sprite::Sprite(void)
	{
		
	}

	void Sprite::setTexture(const std::string& textureNameIn)
	{
		textureName = textureNameIn;
	}

	void Sprite::setDimensions(const Vector2& dimensionsIn)
	{
		vDimensions = dimensionsIn;
	}
}