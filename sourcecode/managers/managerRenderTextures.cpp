#include "precompiled_header.h"
#include "managerRenderTextures.h"
#include "../core/log.h"
#include "../graphics/renderDevice.h"

namespace Nexus
{
	ManagerRenderTextures::ManagerRenderTextures()
	{
		addNewGroup("default");
	}

	unsigned int ManagerRenderTextures::getNumGroups(void)
	{
		return (unsigned int)group.size();
	}

	unsigned int ManagerRenderTextures::getNumResInGroup(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerRenderTextures::getNumResInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			Log::getPointer()->exception(err);
		}
		std::map<std::string, RenderTextureGroup*>::iterator itg = group.find(strGroupName);
		return (unsigned int)itg->second->_mmapResource.size();
	}

	const std::string& ManagerRenderTextures::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= group.size())
		{
			std::string err("ManagerRenderTextures::getGroupName(");

			err.append(std::to_string(iGroupIndex));
			err.append(") failed. Invalid index given. Number of groups is ");
			err.append(std::to_string(getNumGroups()));
			Log::getPointer()->exception(err);
		}
		std::map<std::string, RenderTextureGroup*>::iterator itg = group.begin();
		unsigned int i = 0;
		while (i < iGroupIndex)
		{
			itg++;
			++i;
		}
		return itg->first;
	}

	void ManagerRenderTextures::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			std::string err("ManagerRenderTextures::addNewGroup() has been given the new group name of \"");
			err.append(strNewGroupName);
			err.append("\" but it already exists! Only new groups can be added.");
			Log::getPointer()->exception(err);
		}
		RenderTextureGroup* pNewGroup = new RenderTextureGroup;
		group[strNewGroupName] = pNewGroup;
	}

	bool ManagerRenderTextures::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, RenderTextureGroup*>::iterator it = group.find(strGroupName);
		if (it == group.end())
			return false;
		return true;
	}

	void ManagerRenderTextures::addRenderTexture(const std::string& strNewResourceName, int iWidth, int iHeight, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerRenderTextures::addRenderTexture(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource already exists in the group?
		std::map<std::string, RenderTextureGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, RenderTexture*>::iterator itr = itg->second->_mmapResource.find(strNewResourceName);	// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() != itr)
		{
			// Increase reference count of the resource
			itr->second->refCount++;
			return;
		}

		// If we get here, we have got to create, then add the resource to the existing named group
		RenderTexture* pNewRes = new RenderTexture(iWidth, iHeight);
		itg->second->_mmapResource[strNewResourceName] = pNewRes;
	}

	RenderTexture* ManagerRenderTextures::getRenderTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerRenderTextures::getRenderTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, RenderTextureGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, RenderTexture*>::iterator itr = itg->second->_mmapResource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() == itr)
		{
			std::string err("ManagerRenderTextures::getRenderTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" exists, the named resource couldn't be found!");
			Log::getPointer()->exception(err);
		}
		return (RenderTexture*)itr->second;
	}



	bool ManagerRenderTextures::getExistsRenderTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, RenderTextureGroup*>::iterator itg = group.find(strGroupName);
		if (itg == group.end())
			return false;
		std::map<std::string, RenderTexture*>::iterator itr = itg->second->_mmapResource.find(strResourceName);
		if (itr == itg->second->_mmapResource.end())
			return false;
		return true;
	}


	void ManagerRenderTextures::removeRenderTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerRenderTextures::removeRenderTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, RenderTextureGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, RenderTexture*>::iterator itr = itg->second->_mmapResource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() == itr)
		{
			std::string err("ManagerRenderTextures::removeRenderTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, exists, the named resource couldn't be found!");
			Log::getPointer()->exception(err);
		}

		// If we get here, we've found the resource in the group
		// Decrement it's reference count 
		itr->second->refCount--;
		// If the reference count is now at zero
		if (itr->second->refCount <= 0)
		{
			// Destroy the resource
			delete itr->second;
			itg->second->_mmapResource.erase(itr);
		}
	}

	void ManagerRenderTextures::disableTexturing(void)
	{
		glActiveTexture(GL_TEXTURE7);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE6);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE5);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE4);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE3);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE2);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE1);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE0);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
	}
}