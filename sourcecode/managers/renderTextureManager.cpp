#include "precompiled_header.h"
#include "renderTextureManager.h"
#include "../core/log.h"
#include "../graphics/renderDevice.h"

namespace Nexus
{
	RenderTextureManager::RenderTextureManager()
	{
	}

	unsigned int RenderTextureManager::getNumGroups(void)
	{
		return (unsigned int)group.size();
	}

	unsigned int RenderTextureManager::getNumResInGroup(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("RenderTextureManager::getNumResInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			Log::getPointer()->exception(err);
		}
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);
		return (unsigned int)itg->second->resource.size();
	}

	const std::string& RenderTextureManager::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= group.size())
		{
			std::string err("RenderTextureManager::getGroupName(");

			err.append(std::to_string(iGroupIndex));
			err.append(") failed. Invalid index given. Number of groups is ");
			err.append(std::to_string(getNumGroups()));
			Log::getPointer()->exception(err);
		}
		std::map<std::string, Group*>::iterator itg = group.begin();
		unsigned int i = 0;
		while (i < iGroupIndex)
		{
			itg++;
			++i;
		}
		return itg->first;
	}

	void RenderTextureManager::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			return;
		}
		if (groupExists(strNewGroupName))
		{
			std::string err("RenderTextureManager::addNewGroup() has been given the new group name of \"");
			err.append(strNewGroupName);
			err.append("\" but it already exists! Only new groups can be added.");
			Log::getPointer()->exception(err);
		}
		Group* pNewGroup = new Group;
		group[strNewGroupName] = pNewGroup;
	}

	bool RenderTextureManager::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator it = group.find(strGroupName);
		if (it == group.end())
			return false;
		return true;
	}

	void RenderTextureManager::add(const std::string& strNewResourceName, int iWidth, int iHeight, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("RenderTextureManager::add(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource already exists in the group?
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);										// Get iterator to the group (we know it exists)
		std::map<std::string, RenderTexture*>::iterator itr = itg->second->resource.find(strNewResourceName);	// Try to find the named resource in the group
		if (itg->second->resource.end() != itr)
		{
			// Increase reference count of the resource
			itr->second->refCount++;
			return;
		}

		// If we get here, we have got to create, then add the resource to the existing named group
		RenderTexture* pNewRes = new RenderTexture(iWidth, iHeight);
		itg->second->resource[strNewResourceName] = pNewRes;
	}

	RenderTexture* RenderTextureManager::get(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("RenderTextureManager::get(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);										// Get iterator to the group (we know it exists)
		std::map<std::string, RenderTexture*>::iterator itr = itg->second->resource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->resource.end() == itr)
		{
			std::string err("RenderTextureManager::get(\"");
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

	bool RenderTextureManager::getExists(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);
		if (itg == group.end())
			return false;
		std::map<std::string, RenderTexture*>::iterator itr = itg->second->resource.find(strResourceName);
		if (itr == itg->second->resource.end())
			return false;
		return true;
	}

	void RenderTextureManager::remove(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("RenderTextureManager::remove(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);										// Get iterator to the group (we know it exists)
		std::map<std::string, RenderTexture*>::iterator itr = itg->second->resource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->resource.end() == itr)
		{
			std::string err("RenderTextureManager::remove(\"");
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
			itg->second->resource.erase(itr);
		}
	}

	void RenderTextureManager::disableTexturing(void)
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