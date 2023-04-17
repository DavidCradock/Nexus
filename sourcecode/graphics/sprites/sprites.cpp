#include "precompiled_header.h"
#include "sprites.h"
#include "../texture.h"
#include "../shader.h"
#include "../../core/log.h"

namespace Nexus
{
	Sprite::Sprite(void)
	{
		
		
	}

	void Sprite::render(const Vector2 &vPositionIn)
	{
		Vector3 vPos(vPositionIn.x, vPositionIn.y, 0.0f);
		matrixTransform.setIdentity();
		matrixTransform.setTranslation(vPos);

		Texture* pTexture = TextureManager::getPointer()->get2DTexture(textureName);
		pTexture->bind();

		Shader* pShader = ShaderManager::getPointer()->getShader("sprites");
		//pShader->setInt("texture1", pTexture->getID());

		Matrix matrixOrtho;
		matrixOrtho.setOrthographic();
		pShader->setMat4("transform", matrixOrtho *matrixTransform);
		pShader->use();

		vertexBuffer.reset();
		vertexBuffer.addQuad(Vector2(0, 0), vDimensions);
		vertexBuffer.upload();

		vertexBuffer.draw();
	}

	void Sprite::setTexture(const std::string& textureNameIn)
	{
		textureName = textureNameIn;
	}

	void Sprite::setDimensions(const Vector2& dimensionsIn)
	{
		vDimensions = dimensionsIn;
	}

	void SpriteManagerTmp::addSprite(const std::string& strSpriteName, const std::string& strTextureFilename)
	{
		std::map<std::string, Sprite*>::iterator itr = sprites.find(strSpriteName);
		if (sprites.end() != itr)
		{
			std::string err("SpriteManager::addSprite(\"");
			err.append(strSpriteName);
			err.append("\") failed. As the given named sprite already exists.");
			Log::getPointer()->addException(err);
		}
		Sprite* pNewSprite = new Sprite;
		pNewSprite->setTexture(strTextureFilename);
		sprites[strSpriteName] = pNewSprite;
	}

	Sprite* SpriteManagerTmp::getSprite(const std::string& strSpriteName)
	{
		// Sprite doesn't exist
		std::map<std::string, Sprite*>::iterator itr = sprites.find(strSpriteName);
		if (sprites.end() == itr)
		{
			std::string err("SpriteManager::getSprite(\"");
			err.append(strSpriteName);
			err.append("\"\" failed. The sprite doesn't exist.");
			Log::getPointer()->addException(err);
		}
		return (Sprite*)itr->second;
	}

	bool SpriteManagerTmp::getExists(const std::string& strSpriteName)
	{
		std::map<std::string, Sprite*>::iterator itr = sprites.find(strSpriteName);
		if (sprites.end() == itr)
			return false;
		return true;
	}

	void SpriteManagerTmp::removeSprite(const std::string& strSpriteName)
	{
		std::map<std::string, Sprite*>::iterator itr = sprites.find(strSpriteName);
		if (sprites.end() == itr)
		{
			std::string err("SpriteManager::removeSprite(\"");
			err.append(strSpriteName);
			err.append("\"\" failed. The sprite doesn't exist.");
			Log::getPointer()->addException(err);
		}

		delete itr->second;
		sprites.erase(itr);
	}
}