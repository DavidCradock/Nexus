#include "precompiled_header.h"
#include "spriteLayer.h"
#include "../../core/log.h"
#include "../../core/utils.h"
#include "../colour.h"
#include "spriteDescription.h"
#include "../../managers/managers.h"
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
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = mapSpriteDescAndSprites.find(strSpriteDescription);
		SpriteDescAndEnt* pSpriteDesc = 0;
		if (itsde == mapSpriteDescAndSprites.end())
		{
			// It doesn't, add new description
			pSpriteDesc = new SpriteDescAndEnt;
			if (!pSpriteDesc)
				Log::getPointer()->exception("SpriteLayer::addEntity() memory allocation error");
			mapSpriteDescAndSprites[strSpriteDescription] = pSpriteDesc;
		}
		else
		{
			// If it already exists, then pSpriteDesc will still be null, find it and point to it
			pSpriteDesc = itsde->second;
		}

		// If we get here, pSpriteDesc points to the sprite description struct which holds the sprite entities

		// Does the sprite entity name already exist?
		std::map<std::string, SpriteEntity*>::iterator itse = pSpriteDesc->mapSpriteEntities.find(strSpriteEntityUniqueName);
		if (itse != pSpriteDesc->mapSpriteEntities.end())
		{
			std::string err("SpriteLayer::addEntity() failed. The sprite entity name of ");
			err.append(strSpriteEntityUniqueName);
			err.append(" already exists.");
			Log::getPointer()->exception(err);
		}
		SpriteEntity* pNewSpriteEntity = new SpriteEntity;
		if (!pNewSpriteEntity)
			Log::getPointer()->exception("Memory allocation error");
		pNewSpriteEntity->fRotationRadians = deg2rad(fSpriteRotationDegrees);
		pNewSpriteEntity->vPos = vSpritePosition;
		pNewSpriteEntity->fScale = fSpriteScale;
		pNewSpriteEntity->strSpriteDescName = strSpriteDescription;
		pSpriteDesc->mapSpriteEntities[strSpriteEntityUniqueName] = pNewSpriteEntity;
		return pNewSpriteEntity;
	}

	bool SpriteLayer::entityExists(const std::string& strSpriteEntityUniqueName, const std::string& strSpriteDescription)
	{
		// Does the sprite description already exist?
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = mapSpriteDescAndSprites.find(strSpriteDescription);
		if (itsde == mapSpriteDescAndSprites.end())
			return false;
		// Does the sprite entity name already exist?
		std::map<std::string, SpriteEntity*>::iterator itse = itsde->second->mapSpriteEntities.find(strSpriteEntityUniqueName);
		if (itse == itsde->second->mapSpriteEntities.end())
			return false;
		return true;
	}

	SpriteEntity* SpriteLayer::getEntity(const std::string& strSpriteEntityUniqueName, const std::string& strSpriteDescription)
	{
		// Does the sprite description exist?
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = mapSpriteDescAndSprites.find(strSpriteDescription);
		if (itsde == mapSpriteDescAndSprites.end())
		{
			std::string err("SpriteLayer::getEntity(\"");
			err.append(strSpriteEntityUniqueName);
			err.append("\", \"");
			err.append(strSpriteDescription);
			err.append("\") failed!\nThe sprite description doesn't exist.\n This might be because not a single sprite has been added which uses the sprite description");
			Log::getPointer()->exception(err);
		}
		// Does the sprite entity name exist?
		std::map<std::string, SpriteEntity*>::iterator itse = itsde->second->mapSpriteEntities.find(strSpriteEntityUniqueName);
		if (itse != itsde->second->mapSpriteEntities.end())
			return itse->second;

		std::string err("SpriteLayer::getEntity(\"");
		err.append(strSpriteEntityUniqueName);
		err.append("\", \"");
		err.append(strSpriteDescription);
		err.append("\") failed!\nThe sprite entity doesn't exist!");
		Log::getPointer()->exception(err);
		return 0;	// Keep compiler happy
	}

	void SpriteLayer::removeEntity(const std::string& strSpriteEntityUniqueName, const std::string& strSpriteDescription)
	{
		// Does the sprite description already exist?
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = mapSpriteDescAndSprites.find(strSpriteDescription);
		if (itsde != mapSpriteDescAndSprites.end())
		{
			// Does the sprite entity name exist?
			std::map<std::string, SpriteEntity*>::iterator itse = itsde->second->mapSpriteEntities.find(strSpriteEntityUniqueName);
			if (itse == itsde->second->mapSpriteEntities.end())
			{
				std::string err("SpriteLayer::removeEntity() failed. The sprite entity name of ");
				err.append(strSpriteEntityUniqueName);
				err.append(" doesn't exist.");
				Log::getPointer()->exception(err);
			}
			delete itse->second;
			itse = itsde->second->mapSpriteEntities.erase(itse);
		}
	}

	void SpriteLayer::removeEntityAll(void)
	{
		// For each sprite description (holding sprite entities)
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = mapSpriteDescAndSprites.begin();
		while (itsde != mapSpriteDescAndSprites.end())
		{
			// For each sprite entity
			std::map<std::string, SpriteEntity*>::iterator itse = itsde->second->mapSpriteEntities.begin();
			while (itse != itsde->second->mapSpriteEntities.end())
			{
				delete itse->second;
				itse = itsde->second->mapSpriteEntities.erase(itse);
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

	bool SpriteLayer::hasStuffToRender(void)
	{
		// We have lines?
		if (vecLines.size())
			return true;

		// We have circles?
		if (vecCircles.size())
			return true;

		// For each sprite description
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = mapSpriteDescAndSprites.begin();
		while (itsde != mapSpriteDescAndSprites.end())
		{
			// If this sprite desc has added entities
			if (itsde->second->mapSpriteEntities.size())
				return true;
			itsde++;
		}

		// Nothing to render
		return false;
	}

	void SpriteLayer::render(const Vector2& vCameraPosition, float fCameraZoom)
	{
		timing.update();
		Managers* pMan = Managers::getPointer();

		pMan->textures->disableTexturing();

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float fRenderTargetHeight = float(RenderDevice::getPointer()->getWindowHeight());

		Vector3 v3SpritePos;
		Vector2 v2SpritePos;
		Vector2 vSpriteDims;
		float fSpriteRotRad;

		// For each sprite description
		std::map<std::string, SpriteDescAndEnt*>::iterator itsde = mapSpriteDescAndSprites.begin();
		while (itsde != mapSpriteDescAndSprites.end())
		{
			// Get this layer's sprite description
			SpriteDescription* pDesc = pMan->sprites->getDescription(itsde->first);


			if (0 == pDesc->getFrameTextureName(0).length())
			{
				std::string err("SpriteLayer::_renderEntities() failed!");
				Log::getPointer()->exception(err);
			}
			Texture* pTex = pMan->textures->get(pDesc->getFrameTextureName(0), "sprites");

			// Set description's dimensions to the size of the diffuse texture map
			if (pDesc->nonScaledDims.x == 0 || pDesc->nonScaledDims.y == 0)
			{
				pDesc->nonScaledDims.set((float)pTex->getWidth(), (float)pTex->getHeight());

				// While we're here, compute bounding circle radius
//				pDesc->mfBoundingCircleRadius = float(D3DXVec2Length((D3DXVECTOR2*)&pDesc->mvNonScaledDims)) * 0.5f;
			}

			// Render stuff
//			pSR->begin(vCameraPosition, fCameraZoom);

			Shader* pShader = ShaderManager::getPointer()->get("sprites");
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
			std::map<std::string, SpriteEntity*>::iterator itse = itsde->second->mapSpriteEntities.begin();

			while (itse != itsde->second->mapSpriteEntities.end())
			{
				// Update current frame
				itse->second->fFrameNumber += (float(timing.getSecPast()) * 1000.0f) / pDesc->getFrameDelayMS(unsigned int(itse->second->fFrameNumber));	// CHECK THIS!
				if (itse->second->fFrameNumber >= (float)pDesc->getNumFrames())
					itse->second->fFrameNumber = 0.0f;

				// Compute position and scale of entity
				// Determine whether this sprite entity has a parent, if it has, concatenate parent's position and rotation with this sprite entity
				if (itse->second->bHasParent)
				{
					// Get parent sprite entity
					SpriteLayer* pParentLayer = pMan->sprites->getLayer(itse->second->strParentEntityLayerName);
					SpriteEntity* pParentEntity = pParentLayer->getEntity(itse->second->strParentEntityUniqueName, itse->second->strParentEntitySpriteDesc);

					// Compute combined scale
					vSpriteDims.x = pParentEntity->fScale * itse->second->fScale;
					vSpriteDims.y = vSpriteDims.x;

					// Compute combined rotation
					fSpriteRotRad = pParentEntity->fRotationRadians;
					fSpriteRotRad += itse->second->fRotationRadians;

					// Compute combined position
					v2SpritePos = pParentEntity->vPos;
					Vector2 vVectorToChild = itse->second->vPos;
					vVectorToChild.rotateRad(-pParentEntity->fRotationRadians);
					vVectorToChild.multiply(pParentEntity->fScale);	// Take into consideration scale of parent
					v2SpritePos += vVectorToChild;
					v3SpritePos.x = v2SpritePos.x;
					v3SpritePos.y = v2SpritePos.y;
				}
				else
				{
					v2SpritePos = itse->second->vPos;
					v3SpritePos.x = v2SpritePos.x;
					v3SpritePos.y = v2SpritePos.y;
					fSpriteRotRad = itse->second->fRotationRadians;
					vSpriteDims.x = itse->second->fScale;
					vSpriteDims.y = itse->second->fScale;
				}

				// Render the sprite
				Vector2 renderDims = itse->second->getRenderDims();
				vSpriteDims.x *= renderDims.x;
				vSpriteDims.y *= renderDims.y;

				// Get texture of sprite entity
				pTex = pMan->textures->get(pDesc->getFrameTextureName(itse->second->getCurrentFrameNumber()), "sprites");
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
				itse++;
			}
			itsde++;
		}
	}

	void SpriteLayer::addLine(const Vector2& vLinePos1, const Vector2& vLinePos2, const Colouruc& lineColour1, const Colouruc& lineColour2, float fLineWidth)
	{
		LineRender* pNewLine = new LineRender;
		if (!pNewLine)
			Log::getPointer()->exception("SpriteLayer::addLine() memory allocation error.");
		pNewLine->vPos[0] = vLinePos1;
		pNewLine->vPos[1] = vLinePos2;
		pNewLine->cCol[0] = lineColour1;
		pNewLine->cCol[1] = lineColour2;
		pNewLine->fLineWidth = fLineWidth;
		vecLines.push_back(pNewLine);
	}

	void SpriteLayer::removeAllLines(void)
	{
		for (int i = 0; i < (int)vecLines.size(); ++i)
		{
			delete vecLines[i];
		}
		vecLines.clear();
	}

	void SpriteLayer::addCircle(const Vector2& vCirclePos, float fRadius, const Colouruc& circleColour, float fLineWidth, unsigned int iNumSegments)
	{
		CircleRender* pNewCircle = new CircleRender;
		if (!pNewCircle)
			Log::getPointer()->exception("SpriteLayer::addCircle() memory allocation error.");
		pNewCircle->vPos = vCirclePos;
		pNewCircle->fRadius = fRadius;
		pNewCircle->cCol = circleColour;
		pNewCircle->fLineWidth = fLineWidth;
		pNewCircle->iNumSegments = iNumSegments;
		vecCircles.push_back(pNewCircle);
	}

	void SpriteLayer::removeAllCircles(void)
	{
		for (int i = 0; i < (int)vecCircles.size(); ++i)
		{
			delete vecCircles[i];
		}
		vecCircles.clear();
	}

}