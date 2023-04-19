#include "precompiled_header.h"
#include "sprites.h"
#include "../../managers/managerShaders.h"
#include "../../managers/managerTextures.h"
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

		Shader* pShader = ManagerShaders::getPointer()->getShader("sprites");
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
}