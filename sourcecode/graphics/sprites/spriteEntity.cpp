#include "precompiled_header.h"
#include "spriteEntity.h"
#include "../../core/log.h"
#include "../../managers/managers.h"

namespace Nexus
{

	SpriteEntity::SpriteEntity()
	{
		vPos.setZero();
		fRotationRadians = 0.0f;
		fScale = 1.0f;
		colour.set(255, 255, 255, 255);
		fFrameNumber = 0.0f;

		// Clear parent entity information
		removeParentEntity();
	}

	Vector2 SpriteEntity::getRenderDims(void)
	{
		// Get dims of the texture this sprite is using
		Managers* pMan = Managers::getPointer();

		Texture* pResTex = pMan->textures->get(pMan->sprites->getDescription(strSpriteDescName)->getFrameTextureName(0), "sprites");
		if (!pResTex)
		{
			std::string err("SpriteEntity::getRenderDims() failed to acquire texture from resource manager: ");
			err.append(strSpriteDescName);
			Log::getPointer()->exception(err);
		}
		Vector2 vDims;
		vDims.x = float(pResTex->getWidth());
		vDims.y = float(pResTex->getHeight());
		vDims.x *= fScale;
		vDims.y *= fScale;
		return vDims;
	}

	Vector2 SpriteEntity::getSpriteDescDims(void)
	{
		Managers* pMan = Managers::getPointer();

		Texture* pResTex = pMan->textures->get(pMan->sprites->getDescription(strSpriteDescName)->getFrameTextureName(0), "sprites");
		if (!pResTex)
		{
			std::string err("SpriteEntity::getSpriteDescDims() failed to acquire texture from resource manager: ");
			err.append(strSpriteDescName);
			Log::getPointer()->exception(err);
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

		rectThis.left = (int)vPos.x;
		rectThis.top = (int)vPos.y;
		rectThis.right = int(vPos.x + thisDims.x);
		rectThis.bottom = int(vPos.y + thisDims.y);

		rectOther.left = (int)otherSprite.vPos.x;
		rectOther.top = (int)otherSprite.vPos.y;
		rectOther.right = int(otherSprite.vPos.x + otherDims.x);
		rectOther.bottom = int(otherSprite.vPos.y + otherDims.y);
		return rectThis.collision(rectOther);
	}

	void SpriteEntity::setParentEntity(const std::string& strParentEntityLayerNameIn, const std::string& strParentSpriteEntityUniqueNameIn, const std::string& strParentSpriteDescriptionIn)
	{
		// Store values for later use
		bHasParent = true;
		strParentEntityLayerName = strParentEntityLayerNameIn;
		strParentEntityUniqueName = strParentSpriteEntityUniqueNameIn;
		strParentEntitySpriteDesc = strParentSpriteDescriptionIn;
	}

	void SpriteEntity::removeParentEntity(void)
	{
		bHasParent = false;
		strParentEntityLayerName.clear();
		strParentEntityUniqueName.clear();
		strParentEntitySpriteDesc.clear();
	}

}