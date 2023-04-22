#include "precompiled_header.h"
#include "managerSprites.h"
#include "../core/log.h"
#include "../managers/managerTextures.h"

namespace Nexus
{

	ManagerSprites::ManagerSprites()
	{
		// Add the resource group "sprites"
		ManagerTextures* pManTextures = ManagerTextures::getPointer();
		RenderDevice* pRenderDevice = RenderDevice::getPointer();
		pManTextures->addNewGroup("sprites");

		fCameraZoomCurrent = 1.0f;
		fCameraZoomTarget = 1.0f;
		fCameraZoomSpeed = 0.0f;
		vCameraPositionCurrent.x = pRenderDevice->getWindowWidth() * 0.5f;
		vCameraPositionCurrent.y = pRenderDevice->getWindowHeight() * 0.5f;
		vCameraPositionTarget = vCameraPositionCurrent;
		fCameraPositionSpeed = 0.0f;
	}


	void ManagerSprites::update(void)
	{
		timing.update();

		// Update camera zoom
		float fCameraCurTarDelta = fCameraZoomCurrent - fCameraZoomTarget;
		absolute(fCameraCurTarDelta);
		if (fCameraZoomCurrent < fCameraZoomTarget)
			fCameraZoomCurrent += float(timing.getSecPast()) * fCameraZoomSpeed * fCameraCurTarDelta;
		else
			fCameraZoomCurrent -= float(timing.getSecPast()) * fCameraZoomSpeed * fCameraCurTarDelta;

		// Update camera position
		Vector2 vCameraCurTarDelta;
		vCameraCurTarDelta.x = vCameraPositionCurrent.x - vCameraPositionTarget.x;
		vCameraCurTarDelta.y = vCameraPositionCurrent.y - vCameraPositionTarget.y;
		absolute(vCameraCurTarDelta.x);
		absolute(vCameraCurTarDelta.y);
		if (vCameraPositionCurrent.x < vCameraPositionTarget.x)
			vCameraPositionCurrent.x += float(timing.getSecPast()) * fCameraPositionSpeed * vCameraCurTarDelta.x;
		else
			vCameraPositionCurrent.x -= float(timing.getSecPast()) * fCameraPositionSpeed * vCameraCurTarDelta.x;
		if (vCameraPositionCurrent.y < vCameraPositionTarget.y)
			vCameraPositionCurrent.y += float(timing.getSecPast()) * fCameraPositionSpeed * vCameraCurTarDelta.y;
		else
			vCameraPositionCurrent.y -= float(timing.getSecPast()) * fCameraPositionSpeed * vCameraCurTarDelta.y;

	}

	void ManagerSprites::setCameraPosition(const Vector2& vCameraTargetPosition, float fSpeed)
	{
		vCameraPositionTarget = vCameraTargetPosition;

		// Compute camera movement speed based on initial distance from current and target positions
	//	clamp(fSpeed, 0.0001f, FLT_MAX);
	//	CVector3 vCur(_mvCameraPositionCurrent.x, _mvCameraPositionCurrent.y, 0.0f);
	//	CVector3 vTar(vCameraTargetPosition.x, vCameraTargetPosition.y, 0.0f);
	//	vCur.x -= vTar.x;
	//	vCur.y -= vTar.y;
	//	_mfCameraPositionSpeed = vCur.getLength() * (1.0f / fSpeed);
		fCameraPositionSpeed = fSpeed;
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
		for (size_t i = 0; i < vecLayerNameZOrder.size(); ++i)
		{
			std::map<std::string, SpriteLayer*>::iterator itlayer = mapLayers.find(vecLayerNameZOrder[i]);
			if (itlayer != mapLayers.end())
				itlayer->second->render(vCameraPositionCurrent, fCameraZoomCurrent);
		}
	}

	bool ManagerSprites::hasStuffToRender(void)
	{
		// For each layer
		for (size_t i = 0; i < vecLayerNameZOrder.size(); ++i)
		{
			std::map<std::string, SpriteLayer*>::iterator itlayer = mapLayers.find(vecLayerNameZOrder[i]);
			if (itlayer != mapLayers.end())
			{
				if (itlayer->second->hasStuffToRender())
					return true;
			}
		}

		// If we get here, no layers have stuff needed to be rendered
		return false;
	}

	SpriteLayer* ManagerSprites::addLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, SpriteLayer*>::iterator itlayer = mapLayers.find(strUniqueName);
		if (itlayer != mapLayers.end())
		{
			std::string err("ManagerSprites::addLayer(\"");
			err.append(strUniqueName);
			err.append("\") failed. The layer already exists.");
			Log::getPointer()->exception(err);
		}

		SpriteLayer* pNewLayer = new SpriteLayer;
		if (!pNewLayer)
			Log::getPointer()->exception("ManagerSprites memory allocation error.");

		// Add layer to hash map
		mapLayers[strUniqueName] = pNewLayer;

		// Add layer name to z order vector
		vecLayerNameZOrder.push_back(strUniqueName);

		return pNewLayer;
	}

	bool ManagerSprites::layerExists(const std::string& strUniqueName)
	{
		if (mapLayers.find(strUniqueName) == mapLayers.end())
			return false;
		return true;
	}
	SpriteLayer* ManagerSprites::getLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, SpriteLayer*>::iterator itlayer = mapLayers.find(strUniqueName);
		if (itlayer == mapLayers.end())
		{
			std::string err("ManagerSprites::getLayer(\"");
			err.append(strUniqueName);
			err.append("\") failed. Layer name doesn't exist!");
			Log::getPointer()->exception(err);
		}
		return itlayer->second;
	}

	void ManagerSprites::removeLayer(const std::string& strUniqueName)
	{
		// Attempt to find if the layer name already exists
		std::map<std::string, SpriteLayer*>::iterator itlayer = mapLayers.find(strUniqueName);
		if (itlayer == mapLayers.end())
		{
			std::string err("ManagerSprites::removeLayer(\"");
			err.append(strUniqueName);
			err.append("\") failed. The layer doesn't exist.");
			Log::getPointer()->exception(err);
		}

		SpriteLayer* pLayer = itlayer->second;
		// De-allocate memory for the layer
		delete pLayer;

		// Remove layer from hash map
		itlayer = mapLayers.erase(itlayer);

		// Remove layer name from z order vector
		std::vector<std::string>::iterator itv = vecLayerNameZOrder.begin();
		for (int i = 0; i < (int)vecLayerNameZOrder.size(); ++i)
		{
			if (vecLayerNameZOrder[i] == strUniqueName)
			{
				itv = vecLayerNameZOrder.erase(itv);
				break;
			}
			itv++;
		}
	}

	void ManagerSprites::removeAllLayers(void)
	{
		// Get names of each layer
		std::vector<std::string> vNames;
		std::map<std::string, SpriteLayer*>::iterator itlayer = mapLayers.begin();
		while (itlayer != mapLayers.end())
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
		if (iZorder < 0 || iZorder >= (int)vecLayerNameZOrder.size())
		{
			std::string err("ManagerSprites::getLayerName(");
			err.append(std::to_string(iZorder));
			err.append(") failed. Invalid iZorder value given!");
			Log::getPointer()->exception(err);
		}
		return vecLayerNameZOrder[iZorder];
	}

	int ManagerSprites::getLayerZorder(const std::string& strLayerName)
	{
		// Find current position of the named layer
		int iCurrentIndex = -1;
		for (int i = 0; i < (int)vecLayerNameZOrder.size(); ++i)
		{
			if (vecLayerNameZOrder[i] == strLayerName)
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
			Log::getPointer()->exception(err);
		}
		return iCurrentIndex;
	}

	void ManagerSprites::moveLayerToFrontByOne(const std::string& strLayerName)
	{
		// Find current position of the named layer
		int iCurrentIndex = -1;
		for (int i = 0; i < (int)vecLayerNameZOrder.size(); ++i)
		{
			if (vecLayerNameZOrder[i] == strLayerName)
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
			Log::getPointer()->exception(err);
		}

		// If it's already at the front, do nothing
		if ((int)vecLayerNameZOrder.size() - 1 == iCurrentIndex)
			return;

		// Move it to front by one...
		vecLayerNameZOrder[iCurrentIndex] = vecLayerNameZOrder[iCurrentIndex + 1];
		vecLayerNameZOrder[iCurrentIndex + 1] = strLayerName;
	}

	void ManagerSprites::moveLayerToBackByOne(const std::string& strLayerName)
	{
		// Find current position of the named layer
		int iCurrentIndex = -1;
		for (int i = 0; i < (int)vecLayerNameZOrder.size(); ++i)
		{
			if (vecLayerNameZOrder[i] == strLayerName)
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
			Log::getPointer()->exception(err);
		}

		// If it's already at the back, do nothing
		if (0 == iCurrentIndex)
			return;

		// Move it back one...
		vecLayerNameZOrder[iCurrentIndex] = vecLayerNameZOrder[iCurrentIndex - 1];
		vecLayerNameZOrder[iCurrentIndex - 1] = strLayerName;
	}


	void ManagerSprites::moveLayerToBack(const std::string& strLayerName)
	{
		// Make sure the layer name exists
		bool bFound = false;
		for (int i = 0; i < (int)vecLayerNameZOrder.size(); ++i)
		{
			if (vecLayerNameZOrder[i] == strLayerName)
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
			Log::getPointer()->exception(err);
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
		for (int i = 0; i < (int)vecLayerNameZOrder.size(); ++i)
		{
			if (vecLayerNameZOrder[i] == strLayerName)
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
			Log::getPointer()->exception(err);
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
		for (int i = 0; i < (int)vecLayerNameZOrder.size(); ++i)
		{
			if (vecLayerNameZOrder[i] == strLayerName)
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
			Log::getPointer()->exception(err);
		}
		bFound = false;
		for (int i = 0; i < (int)vecLayerNameZOrder.size(); ++i)
		{
			if (vecLayerNameZOrder[i] == strLayerNameOther)
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
			Log::getPointer()->exception(err);
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
		for (int i = 0; i < (int)vecLayerNameZOrder.size(); ++i)
		{
			if (vecLayerNameZOrder[i] == strLayerName)
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
			Log::getPointer()->exception(err);
		}
		bFound = false;
		for (int i = 0; i < (int)vecLayerNameZOrder.size(); ++i)
		{
			if (vecLayerNameZOrder[i] == strLayerNameOther)
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
			Log::getPointer()->exception(err);
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
		std::map<std::string, SpriteDescription*>::iterator itlayer = mapDescriptions.find(strUniqueName);
		if (itlayer != mapDescriptions.end())
		{
			std::string err("ManagerSprites::addDescription(\"");
			err.append(strUniqueName);
			err.append("\") failed. The description already exists.");
			Log::getPointer()->exception(err);
		}

		SpriteDescription* pNewDesc = new SpriteDescription;
		if (!pNewDesc)
			Log::getPointer()->exception("ManagerSprites memory allocation error.");

		// Add layer to hash map
		mapDescriptions[strUniqueName] = pNewDesc;

		return pNewDesc;
	}

	SpriteDescription* ManagerSprites::getDescription(const std::string& strUniqueName)
	{
		// Attempt to find if the named object already exists
		std::map<std::string, SpriteDescription*>::iterator itdesc = mapDescriptions.find(strUniqueName);
		if (itdesc == mapDescriptions.end())
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
		std::map<std::string, SpriteDescription*>::iterator it = mapDescriptions.begin();
		std::vector<std::string> vNames;
		while (it != mapDescriptions.end())
		{
			delete it->second;
			it = mapDescriptions.erase(it);
		}
		removeAllLayers();
	}

	void ManagerSprites::resetCamera(void)
	{
		fCameraZoomCurrent = 1.0f;
		fCameraZoomTarget = 1.0f;
		fCameraZoomSpeed = 0.0f;
		vCameraPositionCurrent.x = RenderDevice::getPointer()->getWindowWidth() * 0.5f;
		vCameraPositionCurrent.y = RenderDevice::getPointer()->getWindowHeight() * 0.5f;
		vCameraPositionTarget = vCameraPositionCurrent;
		fCameraPositionSpeed = 0.0f;
	}







}