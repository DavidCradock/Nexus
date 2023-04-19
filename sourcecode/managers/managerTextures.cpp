#include "precompiled_header.h"
#include "managerTextures.h"
#include "../core/log.h"
#include "../graphics/image.h"

namespace Nexus
{
	ManagerTextures::ManagerTextures()
	{
		addNewGroup("default");
		addNewGroup("fonts");
		addNewGroup("gui");
	}


	unsigned int ManagerTextures::getNumGroups(void)
	{
		return (unsigned int)group.size();
	}

	unsigned int ManagerTextures::getNumResInGroup(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerTextures::getNumResInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			throw std::runtime_error(err);
		}
		std::map<std::string, TextureGroup*>::iterator itg = group.find(strGroupName);
		size_t iTotal = itg->second->_mmapResource.size();
		//	iTotal += itg->second->_mmapResFont.size();
		return (unsigned int)iTotal;
	}

	unsigned int ManagerTextures::getNumResInGroupLoaded(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerTextures::getNumResInGroupLoaded(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			throw std::runtime_error(err);
		}
		std::map<std::string, TextureGroup*>::iterator itg = group.find(strGroupName);

		unsigned int iResLoadedTotal = 0;
		std::map<std::string, Texture*>::iterator it0 = itg->second->_mmapResource.begin();
		while (it0 != itg->second->_mmapResource.end())
		{
			if (it0->second->getLoaded())
				++iResLoadedTotal;
			it0++;
		}

		return iResLoadedTotal;
	}

	const std::string& ManagerTextures::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= group.size())
		{
			std::string err("ManagerTextures::getGroupName(");

			err.append(std::to_string(iGroupIndex));
			err.append(") failed. Invalid index given. Number of groups is ");
			err.append(std::to_string(getNumGroups()));
			throw std::runtime_error(err);
		}
		std::map<std::string, TextureGroup*>::iterator itg = group.begin();
		unsigned int i = 0;
		while (i < iGroupIndex)
		{
			itg++;
			++i;
		}
		return itg->first;
	}

	void ManagerTextures::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			return;
		}

		TextureGroup* pNewGroup = new TextureGroup;
		group[strNewGroupName] = pNewGroup;
	}

	bool ManagerTextures::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, TextureGroup*>::iterator it = group.find(strGroupName);
		if (it == group.end())
			return false;
		return true;
	}

	void ManagerTextures::loadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerTextures::loadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			throw std::runtime_error(err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, TextureGroup*>::iterator itg = group.find(strGroupName);

		// For each 2d texture resource in this group...
		std::map<std::string, Texture*>::iterator itTexture = itg->second->_mmapResource.begin();
		while (itTexture != itg->second->_mmapResource.end())
		{
			if (!itTexture->second->loaded)
			{
				itTexture->second->load();
				itTexture->second->loaded = true;
			}
			itTexture++;
		}

	}

	void ManagerTextures::loadGroupSingle(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerTextures::loadGroupSingle(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			throw std::runtime_error(err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, TextureGroup*>::iterator itg = group.find(strGroupName);


		// For each 2d texture resource in this group...
		std::map<std::string, Texture*>::iterator itTexture = itg->second->_mmapResource.begin();
		while (itTexture != itg->second->_mmapResource.end())
		{
			if (!itTexture->second->loaded)
			{
				itTexture->second->load();
				itTexture->second->loaded = true;
				return;	// We've changed a resource from unloaded to loaded state, aka, we've loaded a resource, simply return.
			}
			itTexture++;
		}

	}

	void ManagerTextures::unloadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerTextures::unloadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			throw std::runtime_error(err);
		}

		// Unload any loaded resources within the group
		std::map<std::string, TextureGroup*>::iterator itg = group.find(strGroupName);

		// For each 2d texture resource in this group...
		std::map<std::string, Texture*>::iterator itTexture = itg->second->_mmapResource.begin();
		while (itTexture != itg->second->_mmapResource.end())
		{
			if (itTexture->second->loaded)
			{
				itTexture->second->unload();
				itTexture->second->loaded = false;
			}
			itTexture++;
		}
	}


	void ManagerTextures::add2DTexture(const std::string& strNewResourceName, const std::string& strTextureFilename, const std::string& strGroupName, bool bImageFlipOnLoad, TextureFiltering filter)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerTextures::add2DTexture(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist!");
			throw std::runtime_error(err);
		}

		// Resource already exists in the group?
		std::map<std::string, TextureGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, Texture*>::iterator itr = itg->second->_mmapResource.find(strNewResourceName);	// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() != itr)
		{
			itr->second->refCount++;
			return;
		}

		// If we get here, we have got to create, then add the resource to the existing named group
		Texture* pNewRes = new Texture(strTextureFilename, bImageFlipOnLoad, filter);
		itg->second->_mmapResource[strNewResourceName] = pNewRes;
	}

	Texture* ManagerTextures::get2DTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerTextures::get2DTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist!");
			throw std::runtime_error(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, TextureGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, Texture*>::iterator itr = itg->second->_mmapResource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() == itr)
		{
			std::string err("ManagerTextures::get2DTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" exists, the named resource couldn't be found!");
			throw std::runtime_error(err);
		}

		// Is the resource in an unloaded state?
		if (!itr->second->loaded)
		{
			// Load it
			itr->second->load();
			itr->second->loaded = true;
		}
		// Return the resource pointer...
		return (Texture*)itr->second;
	}



	bool ManagerTextures::getExists2DTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, TextureGroup*>::iterator itg = group.find(strGroupName);
		if (itg == group.end())
			return false;
		std::map<std::string, Texture*>::iterator itr = itg->second->_mmapResource.find(strResourceName);
		if (itr == itg->second->_mmapResource.end())
			return false;
		return true;
	}


	void ManagerTextures::remove2DTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerTextures::remove2DTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist!");
			throw std::runtime_error(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, TextureGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, Texture*>::iterator itr = itg->second->_mmapResource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() == itr)
		{
			std::string err("ManagerTextures::remove2DTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, exists, the named resource couldn't be found!");
			throw std::runtime_error(err);
		}

		// If we get here, we've found the resource in the group
		// Decrement it's reference count 
		itr->second->refCount--;
		// If the reference count is now at zero
		if (itr->second->refCount <= 0)
		{
			// If it's in a loaded state, unload it
			if (itr->second->loaded)
			{
				itr->second->unload();
				itr->second->loaded = false;
			}

			// Destroy the resource
			delete itr->second;
			itg->second->_mmapResource.erase(itr);
		}
	}

	void ManagerTextures::disableTexturing(void)
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