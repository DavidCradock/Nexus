#include "precompiled_header.h"
#include "managerSprites.h"
#include "../core/log.h"
#include "../managers/managerTextures.h"

namespace Nexus
{

	ManagerSprites::ManagerSprites()
	{
		// Add the resource group "sprites"
		ManagerTextures* pTM = ManagerTextures::getPointer();
		pTM->addNewGroup("sprites");

		// Add sprite material
//		pRM->addMaterial("SPRITE_MATERIAL_DIFFUSE", "../data/core/materials/matSpriteDiffuse.fx", "CORE");				// Diffuse only

		_mfCameraZoomCurrent = 1.0f;
		_mfCameraZoomTarget = 1.0f;
		_mfCameraZoomSpeed = 0.0f;
		_mvCameraPositionCurrent.x = RenderDevice::getPointer()->getWindowWidth() * 0.5f;
		_mvCameraPositionCurrent.y = RenderDevice::getPointer()->getWindowHeight() * 0.5f;
		_mvCameraPositionTarget = _mvCameraPositionCurrent;
		_mfCameraPositionSpeed = 0.0f;
	}


	void ManagerSprites::update(void)
	{
		_mcTimer.update();

		// Update camera zoom
		float fCameraCurTarDelta = _mfCameraZoomCurrent - _mfCameraZoomTarget;
		absolute(fCameraCurTarDelta);
		if (_mfCameraZoomCurrent < _mfCameraZoomTarget)
			_mfCameraZoomCurrent += float(_mcTimer.getSecPast()) * _mfCameraZoomSpeed * fCameraCurTarDelta;
		else
			_mfCameraZoomCurrent -= float(_mcTimer.getSecPast()) * _mfCameraZoomSpeed * fCameraCurTarDelta;

		// Update camera position
		Vector2 vCameraCurTarDelta;
		vCameraCurTarDelta.x = _mvCameraPositionCurrent.x - _mvCameraPositionTarget.x;
		vCameraCurTarDelta.y = _mvCameraPositionCurrent.y - _mvCameraPositionTarget.y;
		absolute(vCameraCurTarDelta.x);
		absolute(vCameraCurTarDelta.y);
		if (_mvCameraPositionCurrent.x < _mvCameraPositionTarget.x)
			_mvCameraPositionCurrent.x += float(_mcTimer.getSecPast()) * _mfCameraPositionSpeed * vCameraCurTarDelta.x;
		else
			_mvCameraPositionCurrent.x -= float(_mcTimer.getSecPast()) * _mfCameraPositionSpeed * vCameraCurTarDelta.x;
		if (_mvCameraPositionCurrent.y < _mvCameraPositionTarget.y)
			_mvCameraPositionCurrent.y += float(_mcTimer.getSecPast()) * _mfCameraPositionSpeed * vCameraCurTarDelta.y;
		else
			_mvCameraPositionCurrent.y -= float(_mcTimer.getSecPast()) * _mfCameraPositionSpeed * vCameraCurTarDelta.y;

	}

	void ManagerSprites::setCameraPosition(const Vector2& vCameraTargetPosition, float fSpeed)
	{
		_mvCameraPositionTarget = vCameraTargetPosition;

		// Compute camera movement speed based on initial distance from current and target positions
	//	clamp(fSpeed, 0.0001f, FLT_MAX);
	//	CVector3 vCur(_mvCameraPositionCurrent.x, _mvCameraPositionCurrent.y, 0.0f);
	//	CVector3 vTar(vCameraTargetPosition.x, vCameraTargetPosition.y, 0.0f);
	//	vCur.x -= vTar.x;
	//	vCur.y -= vTar.y;
	//	_mfCameraPositionSpeed = vCur.getLength() * (1.0f / fSpeed);
		_mfCameraPositionSpeed = fSpeed;
	}

	void ManagerSprites::render(void)
	{

		// For each layer
	//	std::map<CText, C2DSpriteLayer*>::iterator itlayer = _mmapLayers.begin();
	//	while (itlayer != _mmapLayers.end())
	//	{
	//		itlayer->second->_render();
	//		itlayer++;
	//	}
		// Render each layer based on it's z order
		for (size_t i = 0; i < _mvecLayerNameZOrder.size(); ++i)
		{
			std::map<std::string, SpriteLayer*>::iterator itlayer = _mmapLayers.find(_mvecLayerNameZOrder[i]);
			if (itlayer != _mmapLayers.end())
				itlayer->second->_render(_mvCameraPositionCurrent, _mfCameraZoomCurrent);
		}
	}

	bool ManagerSprites::hasStuffToRender(void)
	{
		// For each layer
		for (size_t i = 0; i < _mvecLayerNameZOrder.size(); ++i)
		{
			std::map<std::string, SpriteLayer*>::iterator itlayer = _mmapLayers.find(_mvecLayerNameZOrder[i]);
			if (itlayer != _mmapLayers.end())
			{
				if (itlayer->second->_hasStuffToRender())
					return true;
			}
		}

		// If we get here, no layers have stuff needed to be rendered
		return false;
	}

	SpriteLayer* ManagerSprites::addLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, SpriteLayer*>::iterator itlayer = _mmapLayers.find(strUniqueName);
		if (itlayer != _mmapLayers.end())
		{
			std::string err("ManagerSprites::addLayer(\"");
			err.append(strUniqueName);
			err.append("\") failed. The layer already exists.");
			throw std::runtime_error(err);
		}

		SpriteLayer* pNewLayer = new SpriteLayer;
		if (!pNewLayer)
			throw std::runtime_error("ManagerSprites memory allocation error.");

		// Add layer to hash map
		_mmapLayers[strUniqueName] = pNewLayer;

		// Add layer name to z order vector
		_mvecLayerNameZOrder.push_back(strUniqueName);

		return pNewLayer;
	}

	bool ManagerSprites::layerExists(const std::string& strUniqueName)
	{
		if (_mmapLayers.find(strUniqueName) == _mmapLayers.end())
			return false;
		return true;
	}
	SpriteLayer* ManagerSprites::getLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, SpriteLayer*>::iterator itlayer = _mmapLayers.find(strUniqueName);
		if (itlayer == _mmapLayers.end())
		{
			std::string err("ManagerSprites::getLayer(\"");
			err.append(strUniqueName);
			err.append("\") failed. Layer name doesn't exist!");
			throw std::runtime_error(err);
		}
		return itlayer->second;
	}

	void ManagerSprites::removeLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, SpriteLayer*>::iterator itlayer = _mmapLayers.find(strUniqueName);
		if (itlayer == _mmapLayers.end())
		{
			std::string err("ManagerSprites::removeLayer(\"");
			err.append(strUniqueName);
			err.append("\") failed. The layer doesn't exist.");
			throw std::runtime_error(err);
		}

		SpriteLayer* pLayer = itlayer->second;
		// De-allocate memory for the layer
		delete pLayer;

		// Remove layer from hash map
		itlayer = _mmapLayers.erase(itlayer);

		// Remove layer name from z order vector
		std::vector<std::string>::iterator itv = _mvecLayerNameZOrder.begin();
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strUniqueName)
			{
				itv = _mvecLayerNameZOrder.erase(itv);
				break;
			}
			itv++;
		}
	}

	void ManagerSprites::removeAllLayers(void)
	{
		// Get names of each layer
		std::vector<std::string> vNames;
		std::map<std::string, SpriteLayer*>::iterator itlayer = _mmapLayers.begin();
		while (itlayer != _mmapLayers.end())
		{
			std::string name = itlayer->first;
			vNames.push_back(name);
			itlayer++;
		}
		for (int i = 0; i < (int)vNames.size(); ++i)
		{
			removeLayer(vNames[i]);
		}
	}

	std::string ManagerSprites::getLayerName(int iZorder)
	{
		// Make sure valid index given
		if (iZorder < 0 || iZorder >= (int)_mvecLayerNameZOrder.size())
		{
			std::string err("ManagerSprites::getLayerName(");
			err.append(std::to_string(iZorder));
			err.append(") failed. Invalid iZorder value given!");
			throw std::runtime_error(err);
		}
		return _mvecLayerNameZOrder[iZorder];
	}

	int ManagerSprites::getLayerZorder(const std::string& strLayerName)
	{
		// Find current position of the named layer
		int iCurrentIndex = -1;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				iCurrentIndex = i;
				break;
			}
			++i;
		}

		// If the layer couldn't be found
		if (-1 == iCurrentIndex)
		{
			std::string err("ManagerSprites::getLayerZorder(\"");
			err.append(strLayerName);
			err.append("\") failed. The given layer name couldn't be found.");
			throw std::runtime_error(err);
		}
		return iCurrentIndex;
	}

	void ManagerSprites::moveLayerToFrontByOne(const std::string& strLayerName)
	{
		// Find current position of the named layer
		int iCurrentIndex = -1;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				iCurrentIndex = i;
				break;
			}
			++i;
		}

		// If the layer couldn't be found
		if (-1 == iCurrentIndex)
		{
			std::string err("ManagerSprites::moveLayerUpOne(\"");
			err.append(strLayerName);
			err.append("\") failed. The given layer name couldn't be found.");
			throw std::runtime_error(err);
		}

		// If it's already at the front, do nothing
		if ((int)_mvecLayerNameZOrder.size() - 1 == iCurrentIndex)
			return;

		// Move it to front by one...
		_mvecLayerNameZOrder[iCurrentIndex] = _mvecLayerNameZOrder[iCurrentIndex + 1];
		_mvecLayerNameZOrder[iCurrentIndex + 1] = strLayerName;
	}

	void ManagerSprites::moveLayerToBackByOne(const std::string& strLayerName)
	{
		// Find current position of the named layer
		int iCurrentIndex = -1;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				iCurrentIndex = i;
				break;
			}
			++i;
		}

		// If the layer couldn't be found
		if (-1 == iCurrentIndex)
		{
			std::string err("ManagerSprites::moveLayerToBackByOne(\"");
			err.append(strLayerName);
			err.append("\") failed. The given layer name couldn't be found.");
			throw std::runtime_error(err);
		}

		// If it's already at the back, do nothing
		if (0 == iCurrentIndex)
			return;

		// Move it back one...
		_mvecLayerNameZOrder[iCurrentIndex] = _mvecLayerNameZOrder[iCurrentIndex - 1];
		_mvecLayerNameZOrder[iCurrentIndex - 1] = strLayerName;
	}


	void ManagerSprites::moveLayerToBack(const std::string& strLayerName)
	{
		// Make sure the layer name exists
		bool bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			std::string err("ManagerSprites::moveLayerToBack(\"");
			err.append(strLayerName);
			err.append("\") failed. The given layer name couldn't be found.");
			throw std::runtime_error(err);
		}
		while (getLayerZorder(strLayerName) != 0)
		{
			moveLayerToBackByOne(strLayerName);
		}

	}

	void ManagerSprites::moveLayerToFront(const std::string& strLayerName)
	{
		// Make sure the layer name exists
		bool bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			std::string err("ManagerSprites::moveLayerToFront(\"");
			err.append(strLayerName);
			err.append("\") failed. The given layer name couldn't be found.");
			throw std::runtime_error(err);
		}
		while (getLayerZorder(strLayerName) != getLayerCount() - 1)
		{
			moveLayerToFrontByOne(strLayerName);
		}
	}

	void ManagerSprites::moveLayerBehind(const std::string& strLayerName, const std::string& strLayerNameOther)
	{
		// Make sure both layer names exist
		bool bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			std::string err("ManagerSprites::moveLayerBehind(\"");
			err.append(strLayerName);
			err.append("\", \"");
			err.append(strLayerNameOther);
			err.append("\") failed. The given layer name couldn't be found. (");
			err.append(strLayerName);
			err.append(")");
			throw std::runtime_error(err);
		}
		bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerNameOther)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			std::string err("ManagerSprites::moveLayerBehind(\"");
			err.append(strLayerName);
			err.append("\", \"");
			err.append(strLayerNameOther);
			err.append("\") failed. The given layer name couldn't be found. (");
			err.append(strLayerNameOther);
			err.append(")");
			throw std::runtime_error(err);
		}

		// If this layer is before other, then move until it's just before
		while (getLayerZorder(strLayerName) < getLayerZorder(strLayerNameOther))
			moveLayerToFrontByOne(strLayerName);

		// If this layer is after other, then move until it's just before
		while (getLayerZorder(strLayerName) > getLayerZorder(strLayerNameOther))
			moveLayerToBackByOne(strLayerName);

	}

	void ManagerSprites::moveLayerInfront(const std::string& strLayerName, const std::string& strLayerNameOther)
	{
		// Make sure both layer names exist
		bool bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerName)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			std::string err("ManagerSprites::moveLayerInfront(\"");
			err.append(strLayerName);
			err.append("\", \"");
			err.append(strLayerNameOther);
			err.append("\") failed. The given layer name couldn't be found. (");
			err.append(strLayerName);
			err.append(")");
			throw std::runtime_error(err);
		}
		bFound = false;
		for (int i = 0; i < (int)_mvecLayerNameZOrder.size(); ++i)
		{
			if (_mvecLayerNameZOrder[i] == strLayerNameOther)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			std::string err("ManagerSprites::moveLayerInfront(\"");
			err.append(strLayerName);
			err.append("\", \"");
			err.append(strLayerNameOther);
			err.append("\") failed. The given layer name couldn't be found. (");
			err.append(strLayerNameOther);
			err.append(")");
			throw std::runtime_error(err);
		}

		// If this layer is after other, then move until it's just before
		while (getLayerZorder(strLayerName) > getLayerZorder(strLayerNameOther))
			moveLayerToBackByOne(strLayerName);

		// If this layer is before other, then move until it's just before
		while (getLayerZorder(strLayerName) < getLayerZorder(strLayerNameOther))
			moveLayerToFrontByOne(strLayerName);
	}

	SpriteDescription* ManagerSprites::addDescription(const std::string& strUniqueName)
	{
		// Attempt to find if the named object already exists
		std::map<std::string, SpriteDescription*>::iterator itlayer = _mmapDescriptions.find(strUniqueName);
		if (itlayer != _mmapDescriptions.end())
		{
			std::string err("ManagerSprites::addDescription(\"");
			err.append(strUniqueName);
			err.append("\") failed. The description already exists.");
			throw std::runtime_error(err);
		}

		SpriteDescription* pNewDesc = new SpriteDescription;
		if (!pNewDesc)
			throw std::runtime_error("ManagerSprites memory allocation error.");

		// Add layer to hash map
		_mmapDescriptions[strUniqueName] = pNewDesc;

		return pNewDesc;
	}

	SpriteDescription* ManagerSprites::getDescription(const std::string& strUniqueName)
	{
		// Attempt to find if the named object already exists
		std::map<std::string, SpriteDescription*>::iterator itdesc = _mmapDescriptions.find(strUniqueName);
		if (itdesc == _mmapDescriptions.end())
			return 0;
		return itdesc->second;
	}

	/*
	CVector2 ManagerSprites::convertScreenspaceToWorldspace(const CVector2& vScreenSpacePosition)
	{
		SCRenderDevice* pRD = SCRenderDevice::getPointer();

		// First, compute matrices
		// As the sprite manager uses SCSpriteRenderer in core/renderSprite.h
		// This is very similar to the matrix setup within that class's begin() method,
		// If we change that code, we have to change this code otherwise things won't be correct
		D3DXMATRIX matWorld;
		D3DXMATRIX matView;
		D3DXMATRIX matProj;
		D3DVIEWPORT9 vp;
		vp.Height = pRD->getWindowHeight();
		vp.Width = pRD->getWindowWidth();
		vp.MaxZ = 1.0f;
		vp.MinZ = 0.0f;
		vp.X = 0;
		vp.Y = 0;

		// World...
		D3DXMatrixIdentity(&matWorld);

		// View...
		CVector3 vEye(_mvCameraPositionCurrent.x, -_mvCameraPositionCurrent.y, _mfCameraZoomCurrent);
		CVector3 vAt = vEye;
		vAt.z = 0;
		CVector3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtRH(&matView, &vEye, &vAt, &vUp);

		// Projection...
		CVector2 vWndHalfDims = pRD->getWindowCenterPosition();
		D3DXMatrixOrthoOffCenterRH(&matProj,
			-vWndHalfDims.x * _mfCameraZoomCurrent,	// l
			vWndHalfDims.x * _mfCameraZoomCurrent,	// r
			vWndHalfDims.y * _mfCameraZoomCurrent,	// b
			-vWndHalfDims.y * _mfCameraZoomCurrent,	// t
			0,										// z near
			100.0f									// z far
		);

		CVector3 v3Screenspace(vScreenSpacePosition.x, vScreenSpacePosition.y, 0.0f);
		CVector3 v3Worldspace;
		CVector2 v2Worldspace;

		// Here's the magic!
		D3DXVec3Unproject(&v3Worldspace, &v3Screenspace, &vp, &matProj, &matView, &matWorld);
		v2Worldspace.x = v3Worldspace.x;
		v2Worldspace.y = -v3Worldspace.y;
		return v2Worldspace;
	}

	CVector2 SC2DSpriteManager::convertWorldspaceToScreenspace(const CVector2& vWorldSpacePosition)
	{
		SCRenderDevice* pRD = SCRenderDevice::getPointer();

		// First, compute matrices
		// As the sprite manager uses SCSpriteRenderer in core/renderSprite.h
		// This is very similar to the matrix setup within that class's begin() method,
		// If we change that code, we have to change this code otherwise things won't be correct
		D3DXMATRIX matWorld;
		D3DXMATRIX matView;
		D3DXMATRIX matProj;
		D3DVIEWPORT9 vp;
		vp.Height = pRD->getWindowHeight();
		vp.Width = pRD->getWindowWidth();
		vp.MaxZ = 1.0f;
		vp.MinZ = 0.0f;
		vp.X = 0;
		vp.Y = 0;

		// World...
		D3DXMatrixIdentity(&matWorld);

		// View...
		CVector3 vEye(_mvCameraPositionCurrent.x, -_mvCameraPositionCurrent.y, _mfCameraZoomCurrent);
		CVector3 vAt = vEye;
		vAt.z = 0;
		CVector3 vUp(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtRH(&matView, &vEye, &vAt, &vUp);

		// Projection...
		CVector2 vWndHalfDims = pRD->getWindowCenterPosition();
		D3DXMatrixOrthoOffCenterRH(&matProj,
			-vWndHalfDims.x * _mfCameraZoomCurrent,	// l
			vWndHalfDims.x * _mfCameraZoomCurrent,	// r
			vWndHalfDims.y * _mfCameraZoomCurrent,	// b
			-vWndHalfDims.y * _mfCameraZoomCurrent,	// t
			0,										// z near
			100.0f									// z far
		);

		CVector3 v3Screenspace;
		CVector3 v3Worldspace(vWorldSpacePosition.x, -vWorldSpacePosition.y, 0.0f);
		CVector2 v2Screenspace;

		// Here's the magic!
		D3DXVec3Project(&v3Screenspace, &v3Worldspace, &vp, &matProj, &matView, &matWorld);
		v2Screenspace.x = v3Screenspace.x;
		v2Screenspace.y = v3Screenspace.y;
		return v2Screenspace;
	}
	*/

	void ManagerSprites::removeAll(void)
	{
		// Remove all sprite descriptions
		std::map<std::string, SpriteDescription*>::iterator it = _mmapDescriptions.begin();
		std::vector<std::string> vNames;
		while (it != _mmapDescriptions.end())
		{
			delete it->second;
			it = _mmapDescriptions.erase(it);
		}
		removeAllLayers();
	}

	void ManagerSprites::resetCamera(void)
	{
		_mfCameraZoomCurrent = 1.0f;
		_mfCameraZoomTarget = 1.0f;
		_mfCameraZoomSpeed = 0.0f;
		_mvCameraPositionCurrent.x = RenderDevice::getPointer()->getWindowWidth() * 0.5f;
		_mvCameraPositionCurrent.y = RenderDevice::getPointer()->getWindowHeight() * 0.5f;
		_mvCameraPositionTarget = _mvCameraPositionCurrent;
		_mfCameraPositionSpeed = 0.0f;
	}







}