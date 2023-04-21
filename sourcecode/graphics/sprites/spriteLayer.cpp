#include "precompiled_header.h"
#include "spriteLayer.h"
#include "../../core/log.h"
#include "../../core/utils.h"
#include "../colour.h"
#include "spriteDescription.h"
#include "../../managers/managerShaders.h"
#include "../../managers/managerSprites.h"
#include "../../managers/managerTextures.h"
#include "../../math/vector4.h"
#include "../vertexBuffer.h"

namespace Nexus
{

	SpriteLayer::SpriteLayer()
	{

	}

	SpriteLayer::~SpriteLayer()
	{
		removeEntityAll();
		removeAllLines();
		removeAllCircles();
	}

	SpriteEntity* SpriteLayer::addEntity(const std::string& strSpriteEntityUniqueName, const std::string& strSpriteDescription, Vector2 vSpritePosition, float fSpriteRotationDegrees, float fSpriteScale)
	{
		// Does the sprite description already exist?
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = _mmapSpriteDescAndSprites.find(strSpriteDescription);
		SpriteDescAndEnt* pSpriteDesc = 0;
		if (itsde == _mmapSpriteDescAndSprites.end())
		{
			// It doesn't, add new description
			pSpriteDesc = new SpriteDescAndEnt;
			if (!pSpriteDesc)
				throw std::runtime_error("Memory allocation error");
			_mmapSpriteDescAndSprites[strSpriteDescription] = pSpriteDesc;
		}
		else
		{
			// If it already exists, then pSpriteDesc will still be null, find it and point to it
			pSpriteDesc = itsde->second;
		}

		// If we get here, pSpriteDesc points to the sprite description struct which holds the sprite entities

		// Does the sprite entity name already exist?
		std::map<std::string, SpriteEntity*>::iterator itse = pSpriteDesc->_mmapSpriteEntities.find(strSpriteEntityUniqueName);
		if (itse != pSpriteDesc->_mmapSpriteEntities.end())
		{
			std::string err("SpriteLayer::addEntity() failed. The sprite entity name of ");
			err.append(strSpriteEntityUniqueName);
			err.append(" already exists.");
			throw std::runtime_error(err);
		}
		SpriteEntity* pNewSpriteEntity = new SpriteEntity;
		if (!pNewSpriteEntity)
			throw std::runtime_error("Memory allocation error");
		pNewSpriteEntity->mfRotationRadians = deg2rad(fSpriteRotationDegrees);
		pNewSpriteEntity->mvPos = vSpritePosition;
		pNewSpriteEntity->mfScale = fSpriteScale;
		pNewSpriteEntity->mStrSpriteDescName = strSpriteDescription;
		pSpriteDesc->_mmapSpriteEntities[strSpriteEntityUniqueName] = pNewSpriteEntity;
		return pNewSpriteEntity;
	}

	bool SpriteLayer::entityExists(const std::string& strSpriteEntityUniqueName, const std::string& strSpriteDescription)
	{
		// Does the sprite description already exist?
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = _mmapSpriteDescAndSprites.find(strSpriteDescription);
		if (itsde == _mmapSpriteDescAndSprites.end())
			return false;
		// Does the sprite entity name already exist?
		std::map<std::string, SpriteEntity*>::iterator itse = itsde->second->_mmapSpriteEntities.find(strSpriteEntityUniqueName);
		if (itse == itsde->second->_mmapSpriteEntities.end())
			return false;
		return true;
	}

	SpriteEntity* SpriteLayer::getEntity(const std::string& strSpriteEntityUniqueName, const std::string& strSpriteDescription)
	{
		// Does the sprite description exist?
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = _mmapSpriteDescAndSprites.find(strSpriteDescription);
		if (itsde == _mmapSpriteDescAndSprites.end())
		{
			std::string err("SpriteLayer::getEntity(\"");
			err.append(strSpriteEntityUniqueName);
			err.append("\", \"");
			err.append(strSpriteDescription);
			err.append("\") failed!\nThe sprite description doesn't exist.\n This might be because not a single sprite has been added which uses the sprite description");
			throw std::runtime_error(err);
		}
		// Does the sprite entity name exist?
		std::map<std::string, SpriteEntity*>::iterator itse = itsde->second->_mmapSpriteEntities.find(strSpriteEntityUniqueName);
		if (itse != itsde->second->_mmapSpriteEntities.end())
			return itse->second;

		std::string err("SpriteLayer::getEntity(\"");
		err.append(strSpriteEntityUniqueName);
		err.append("\", \"");
		err.append(strSpriteDescription);
		err.append("\") failed!\nThe sprite entity doesn't exist!");
		throw std::runtime_error(err);
		return 0;	// Keep compiler happy

	}

	void SpriteLayer::removeEntity(const std::string& strSpriteEntityUniqueName, const std::string& strSpriteDescription)
	{
		// Does the sprite description already exist?
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = _mmapSpriteDescAndSprites.find(strSpriteDescription);
		if (itsde != _mmapSpriteDescAndSprites.end())
		{
			// Does the sprite entity name exist?
			std::map<std::string, SpriteEntity*>::iterator itse = itsde->second->_mmapSpriteEntities.find(strSpriteEntityUniqueName);
			if (itse == itsde->second->_mmapSpriteEntities.end())
			{
				std::string err("SpriteLayer::removeEntity() failed. The sprite entity name of ");
				err.append(strSpriteEntityUniqueName);
				err.append(" doesn't exist.");
				throw std::runtime_error(err);
			}
			delete itse->second;
			itse = itsde->second->_mmapSpriteEntities.erase(itse);
		}
	}

	void SpriteLayer::removeEntityAll(void)
	{
		// For each sprite description (holding sprite entities)
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = _mmapSpriteDescAndSprites.begin();
		while (itsde != _mmapSpriteDescAndSprites.end())
		{
			// For each sprite entity
			std::map<std::string, SpriteEntity*>::iterator itse = itsde->second->_mmapSpriteEntities.begin();
			while (itse != itsde->second->_mmapSpriteEntities.end())
			{
				delete itse->second;
				itse = itsde->second->_mmapSpriteEntities.erase(itse);
			}
			itsde++;
		}
	}

	/* old
	void C2DSpriteLayer::_renderEntities(void)
	{
		SCRenderDevice::getPointer()->disableTexturing();
		SC2DQuadRenderer *p2dq = SC2DQuadRenderer::getPointer();
		SCResourceManager *pRM = SCResourceManager::getPointer();

		// Get the 2D sprite rendering material
		CResMaterial *pMaterial;
		pMaterial = pRM->getMaterial("SPRITE_MATERIAL_DIFFUSE", "CORE");

		float fRenderTargetHeight = float(SCRenderDevice::getPointer()->getWindowHeight());

		// For each sprite description
		std::map<CText, S2D_SpriteDescAndEnt*>::iterator itsde = _mmapSpriteDescAndSprites.begin();
		while (itsde != _mmapSpriteDescAndSprites.end())
		{
			// Get this layer's sprite description
			C2DSpriteDesc *pDesc = SC2DSpriteManager::getPointer()->getDescription(itsde->first);

			// Bind diffuse texture
			if (pDesc->getDiffuseTextureName().length()!= 0)
			{
				pMaterial->setTexture("gTexDiffuseMap", pDesc->getDiffuseTextureName(), "sprites");

				// Set description's dimensions to the size of the diffuse texture map
				if (pDesc->mvNonScaledDims.x == 0 || pDesc->mvNonScaledDims.y == 0)
				{
					CRes2DTexture *pTex = pRM->get2DTexture(pDesc->getDiffuseTextureName(), "sprites");
					pDesc->mvNonScaledDims.set((float)pTex->getWidth(), (float)pTex->getHeight());

					// While we're here, compute bounding circle radius
					pDesc->mfBoundingCircleRadius = float(D3DXVec2Length((D3DXVECTOR2*)&pDesc->mvNonScaledDims)) * 0.5f;
				}
			}

			// Render stuff
			pMaterial->begin();
			while (pMaterial->beginNextPass())
			{
				p2dq->begin(fRenderTargetHeight);

				std::map<CText, C2DSpriteEntity*>::iterator itse = itsde->second->_mmapSpriteEntities.begin();
				CVector2 vSpriteDims;
				while (itse != itsde->second->_mmapSpriteEntities.end())
				{
					vSpriteDims = pDesc->mvNonScaledDims;
					vSpriteDims *= itse->second->mfScale;
					p2dq->addQuad(itse->second->mvPos, vSpriteDims, itse->second->mvColour, CVector2(0,0), CVector2(1,1), itse->second->mfRotationRadians);
					itse++;
				}
				p2dq->end();
			}
			itsde++;
		}

	}
	*/

	bool SpriteLayer::_hasStuffToRender(void)
	{
		// We have lines?
		if (mvecLines.size())
			return true;

		// We have circles?
		if (mvecCircles.size())
			return true;

		// For each sprite description
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = _mmapSpriteDescAndSprites.begin();
		while (itsde != _mmapSpriteDescAndSprites.end())
		{
			// If this sprite desc has added entities
			if (itsde->second->_mmapSpriteEntities.size())
				return true;
			itsde++;
		}

		// Nothing to render
		return false;
	}

	void SpriteLayer::_render(const Vector2& vCameraPosition, float fCameraZoom)
	{
		_mcTimer.update();

		ManagerTextures* pTM = ManagerTextures::getPointer();
		pTM->disableTexturing();

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//		SCSpriteRenderer* pSR = SCSpriteRenderer::getPointer();

		float fRenderTargetHeight = float(RenderDevice::getPointer()->getWindowHeight());

		Vector3 v3SpritePos;
		Vector2 v2SpritePos;
		Vector2 vSpriteDims;
		float fSpriteRotRad;
		ManagerSprites* pManSprites = ManagerSprites::getPointer();

		// For each sprite description
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = _mmapSpriteDescAndSprites.begin();
		while (itsde != _mmapSpriteDescAndSprites.end())
		{
			// Get this layer's sprite description
			SpriteDescription* pDesc = pManSprites->getDescription(itsde->first);


			if (0 == pDesc->getFrameTextureName(0).length())
			{
				std::string err("SpriteLayer::_renderEntities() failed!");
				throw std::runtime_error(err);
			}
			Texture* pTex = pTM->get2DTexture(pDesc->getFrameTextureName(0), "sprites");

			// Set description's dimensions to the size of the diffuse texture map
			if (pDesc->mvNonScaledDims.x == 0 || pDesc->mvNonScaledDims.y == 0)
			{
				pDesc->mvNonScaledDims.set((float)pTex->getWidth(), (float)pTex->getHeight());

				// While we're here, compute bounding circle radius
//				pDesc->mfBoundingCircleRadius = float(D3DXVec2Length((D3DXVECTOR2*)&pDesc->mvNonScaledDims)) * 0.5f;
			}

			// Render stuff
//			pSR->begin(vCameraPosition, fCameraZoom);

			Shader* pShader = ManagerShaders::getPointer()->getShader("sprites");
			Matrix matrixOrtho;
			matrixOrtho.setOrthographic();
			Matrix matrixTranslation;
			Matrix matrixRotation;
			Matrix matrixTransform;
			matrixTranslation.setIdentity();
			matrixRotation.setIdentity();

			//pShader->setInt("texture1", pTexture->getID());
			//pShader->setMat4("transform", matrixOrtho * matrixTransform);
			pShader->use();
			VertexBuffer vertexBuffer;

			// For each sprite entity of this sprite description
			std::map<std::string, SpriteEntity*>::iterator itse = itsde->second->_mmapSpriteEntities.begin();

			while (itse != itsde->second->_mmapSpriteEntities.end())
			{
				// Update current frame
				itse->second->mfFrameNumber += (float(_mcTimer.getSecPast()) * 1000.0f) / pDesc->getFrameDelayMS(unsigned int(itse->second->mfFrameNumber));	// CHECK THIS!
				if (itse->second->mfFrameNumber >= (float)pDesc->getNumFrames())
					itse->second->mfFrameNumber = 0.0f;

				// Compute position and scale of entity
				// Determine whether this sprite entity has a parent, if it has, concatenate parent's position and rotation with this sprite entity
				if (itse->second->mbHasParent)
				{
					// Get parent sprite entity
					SpriteLayer* pParentLayer = pManSprites->getLayer(itse->second->mstrParentEntityLayerName);
					SpriteEntity* pParentEntity = pParentLayer->getEntity(itse->second->mstrParentEntityUniqueName, itse->second->mstrParentEntitySpriteDesc);

					// Compute combined scale
					vSpriteDims.x = pParentEntity->mfScale * itse->second->mfScale;
					vSpriteDims.y = vSpriteDims.x;

					// Compute combined rotation
					fSpriteRotRad = pParentEntity->mfRotationRadians;
					fSpriteRotRad += itse->second->mfRotationRadians;

					// Compute combined position
					v2SpritePos = pParentEntity->mvPos;
					Vector2 vVectorToChild = itse->second->mvPos;
					vVectorToChild.rotateRad(-pParentEntity->mfRotationRadians);
					vVectorToChild.multiply(pParentEntity->mfScale);	// Take into consideration scale of parent
					v2SpritePos += vVectorToChild;
					v3SpritePos.x = v2SpritePos.x;
					v3SpritePos.y = v2SpritePos.y;
				}
				else
				{
					v2SpritePos = itse->second->mvPos;
					v3SpritePos.x = v2SpritePos.x;
					v3SpritePos.y = v2SpritePos.y;
					fSpriteRotRad = itse->second->mfRotationRadians;
					vSpriteDims.x = itse->second->mfScale;
					vSpriteDims.y = itse->second->mfScale;
				}

				// Render the sprite
				Vector2 renderDims = itse->second->getRenderDims();
				vSpriteDims.x *= renderDims.x;
				vSpriteDims.y *= renderDims.y;

				// Get texture of sprite entity
				pTex = pTM->get2DTexture(pDesc->getFrameTextureName(itse->second->getCurrentFrameNumber()), "sprites");
				pTex->bind();
				matrixTranslation.setTranslation(v2SpritePos.x - (0.5f * renderDims.x), v2SpritePos.y - (0.5f * renderDims.y), 0.0f);
				matrixRotation.setFromAxisAngle(Vector3(0, 0, -1), fSpriteRotRad);
				matrixTransform.setIdentity();
				matrixTransform *= matrixTranslation;
				matrixTransform *= matrixRotation;

				pShader->setInt("texture1", pTex->getID());
				pShader->setMat4("transform", matrixOrtho * matrixTransform);
				vertexBuffer.reset();
				vertexBuffer.addQuad(Vector2(-(0.5f * renderDims.x), - (0.5f * renderDims.y)), vSpriteDims);
				vertexBuffer.upload();
				vertexBuffer.draw(false);

//				pSR->addSprite(pTex, v3SpritePos, vSpriteDims, fSpriteRotRad, itse->second->getColour());
				itse++;
			}
			//			pSR->end();
			itsde++;
		}
	}

	void SpriteLayer::addLine(const Vector2& vLinePos1, const Vector2& vLinePos2, const CColouruc& lineColour1, const CColouruc& lineColour2, float fLineWidth)
	{
		SLineRender* pNewLine = new SLineRender;
		if (!pNewLine)
			throw std::runtime_error("Memory allocation error.");
		pNewLine->vPos[0] = vLinePos1;
		pNewLine->vPos[1] = vLinePos2;
		pNewLine->cCol[0] = lineColour1;
		pNewLine->cCol[1] = lineColour2;
		pNewLine->fLineWidth = fLineWidth;
		mvecLines.push_back(pNewLine);
	}

	void SpriteLayer::removeAllLines(void)
	{
		for (int i = 0; i < (int)mvecLines.size(); ++i)
		{
			delete mvecLines[i];
		}
		mvecLines.clear();
	}

	void SpriteLayer::addCircle(const Vector2& vCirclePos, float fRadius, const CColouruc& circleColour, float fLineWidth, unsigned int iNumSegments)
	{
		SCircleRender* pNewCircle = new SCircleRender;
		if (!pNewCircle)
			throw std::runtime_error("Memory allocation error.");
		pNewCircle->vPos = vCirclePos;
		pNewCircle->fRadius = fRadius;
		pNewCircle->cCol = circleColour;
		pNewCircle->fLineWidth = fLineWidth;
		pNewCircle->iNumSegments = iNumSegments;
		mvecCircles.push_back(pNewCircle);
	}

	void SpriteLayer::removeAllCircles(void)
	{
		for (int i = 0; i < (int)mvecCircles.size(); ++i)
		{
			delete mvecCircles[i];
		}
		mvecCircles.clear();
	}

}