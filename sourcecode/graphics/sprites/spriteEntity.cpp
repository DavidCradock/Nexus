#include "precompiled_header.h"
#include "spriteEntity.h"
#include "../../core/log.h"
#include "../../managers/managerSprites.h"
#include "../../managers/managerTextures.h"

namespace Nexus
{

	SpriteEntity::SpriteEntity()
	{
		mvPos.setZero();
		mfRotationRadians = 0.0f;
		mfScale = 1.0f;
		mvColour.set(255, 255, 255, 255);
		mfFrameNumber = 0.0f;

		// Clear parent entity information
		removeParentEntity();
	}

	Vector2 SpriteEntity::getRenderDims(void)
	{
		// Get dims of the texture this sprite is using
		SpriteManager* pSM = SpriteManager::getPointer();

		Texture* pResTex = TextureManager::getPointer()->get2DTexture(pSM->getDescription(mStrSpriteDescName)->getFrameTextureName(0), "sprites");
		if (!pResTex)
		{
			std::string err("SpriteEntity::getRenderDims() failed to acquire texture from resource manager: ");
			err.append(mStrSpriteDescName);
			throw std::runtime_error(err);
		}
		Vector2 vDims;
		vDims.x = float(pResTex->getWidth());
		vDims.y = float(pResTex->getHeight());
		vDims.x *= mfScale;
		vDims.y *= mfScale;
		return vDims;
	}

	Vector2 SpriteEntity::getSpriteDescDims(void)
	{
		SpriteManager* pSM = SpriteManager::getPointer();

		Texture* pResTex = TextureManager::getPointer()->get2DTexture(pSM->getDescription(mStrSpriteDescName)->getFrameTextureName(0), "sprites");
		if (!pResTex)
		{
			std::string err("SpriteEntity::getSpriteDescDims() failed to acquire texture from resource manager: ");
			err.append(mStrSpriteDescName);
			throw std::runtime_error(err);
		}
		Vector2 vDims;
		vDims.x = float(pResTex->getWidth());
		vDims.y = float(pResTex->getHeight());
		return vDims;
	}

	bool SpriteEntity::getRectBoundsCollision(SpriteEntity& otherSprite)
	{
		Rect2D rectThis;
		Rect2D rectOther;
		Vector2 thisDims = getRenderDims();
		Vector2 otherDims = otherSprite.getRenderDims();

		rectThis.left = (int)mvPos.x;
		rectThis.top = (int)mvPos.y;
		rectThis.right = int(mvPos.x + thisDims.x);
		rectThis.bottom = int(mvPos.y + thisDims.y);

		rectOther.left = (int)otherSprite.mvPos.x;
		rectOther.top = (int)otherSprite.mvPos.y;
		rectOther.right = int(otherSprite.mvPos.x + otherDims.x);
		rectOther.bottom = int(otherSprite.mvPos.y + otherDims.y);
		return rectThis.collision(rectOther);
	}

	void SpriteEntity::setParentEntity(const std::string& strParentEntityLayerName, const std::string& strParentSpriteEntityUniqueName, const std::string& strParentSpriteDescription)
	{
		// Store values for later use
		mbHasParent = true;
		mstrParentEntityLayerName = strParentEntityLayerName;
		mstrParentEntityUniqueName = strParentSpriteEntityUniqueName;
		mstrParentEntitySpriteDesc = strParentSpriteDescription;
	}

	void SpriteEntity::removeParentEntity(void)
	{
		mbHasParent = false;
		mstrParentEntityLayerName.clear();
		mstrParentEntityUniqueName.clear();
		mstrParentEntitySpriteDesc.clear();
	}

}