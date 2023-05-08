#include "precompiled_header.h"
#include "textureManager.h"
#include "../core/log.h"
#include "../graphics/image.h"

namespace Nexus
{
	TextureManager::TextureManager()
	{
	}

	unsigned int TextureManager::getNumGroups(void)
	{
		return (unsigned int)group.size();
	}

	unsigned int TextureManager::getNumResInGroup(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::getNumResInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			Log::getPointer()->exception(err);
		}
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);
		return (unsigned int)itg->second->resource.size();;
	}

	unsigned int TextureManager::getNumResInGroupLoaded(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::getNumResInGroupLoaded(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			Log::getPointer()->exception(err);
		}
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);

		unsigned int iResLoadedTotal = 0;
		std::map<std::string, Texture*>::iterator it = itg->second->resource.begin();
		while (it != itg->second->resource.end())
		{
			if (it->second->getLoaded())
				++iResLoadedTotal;
			it++;
		}
		return iResLoadedTotal;
	}

	const std::string& TextureManager::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= group.size())
		{
			std::string err("TextureManager::getGroupName(");

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

	void TextureManager::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			return;
		}
		Group* pNewGroup = new Group;
		group[strNewGroupName] = pNewGroup;
	}

	bool TextureManager::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator it = group.find(strGroupName);
		if (it == group.end())
			return false;
		return true;
	}

	void TextureManager::loadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::loadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			Log::getPointer()->exception(err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);

		// For each 2d texture resource in this group...
		std::map<std::string, Texture*>::iterator it = itg->second->resource.begin();
		while (it != itg->second->resource.end())
		{
			if (!it->second->loaded)
			{
				it->second->load();
				it->second->loaded = true;
			}
			it++;
		}
	}

	void TextureManager::loadGroupSingle(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::loadGroupSingle(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			Log::getPointer()->exception(err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);

		// For each 2d texture resource in this group...
		std::map<std::string, Texture*>::iterator it = itg->second->resource.begin();
		while (it != itg->second->resource.end())
		{
			if (!it->second->loaded)
			{
				it->second->load();
				it->second->loaded = true;
				return;	// We've changed a resource from unloaded to loaded state, aka, we've loaded a resource, simply return.
			}
			it++;
		}
	}

	void TextureManager::unloadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::unloadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			Log::getPointer()->exception(err);
		}

		// Unload any loaded resources within the group
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);

		// For each 2d texture resource in this group...
		std::map<std::string, Texture*>::iterator it = itg->second->resource.begin();
		while (it != itg->second->resource.end())
		{
			if (it->second->loaded)
			{
				it->second->unload();
				it->second->loaded = false;
			}
			it++;
		}
	}

	void TextureManager::add(const std::string& strNewResourceName, const std::string& strTextureFilename, const std::string& strGroupName, bool bImageFlipOnLoad, TextureFiltering filter)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::add(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource already exists in the group?
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);									// Get iterator to the group (we know it exists)
		std::map<std::string, Texture*>::iterator it = itg->second->resource.find(strNewResourceName);	// Try to find the named resource in the group
		if (itg->second->resource.end() != it)
		{
			it->second->refCount++;
			return;
		}

		// If we get here, we have got to create, then add the resource to the existing named group
		Texture* pNewRes = new Texture(strTextureFilename, bImageFlipOnLoad, filter);
		itg->second->resource[strNewResourceName] = pNewRes;
	}

	Texture* TextureManager::get(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::get(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, Texture*>::iterator it = itg->second->resource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->resource.end() == it)
		{
			std::string err("TextureManager::get(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" exists, the named resource couldn't be found!");
			Log::getPointer()->exception(err);
		}

		// Is the resource in an unloaded state?
		if (!it->second->loaded)
		{
			// Load it
			it->second->load();
			it->second->loaded = true;
		}
		// Return the resource pointer...
		return (Texture*)it->second;
	}

	bool TextureManager::getExists(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);
		if (itg == group.end())
			return false;
		std::map<std::string, Texture*>::iterator it = itg->second->resource.find(strResourceName);
		if (it == itg->second->resource.end())
			return false;
		return true;
	}

	void TextureManager::remove(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::remove(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);									// Get iterator to the group (we know it exists)
		std::map<std::string, Texture*>::iterator it = itg->second->resource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->resource.end() == it)
		{
			std::string err("TextureManager::remove(\"");
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
		it->second->refCount--;
		// If the reference count is now at zero
		if (it->second->refCount <= 0)
		{
			// If it's in a loaded state, unload it
			if (it->second->loaded)
			{
				it->second->unload();
				it->second->loaded = false;
			}

			// Destroy the resource
			delete it->second;
			itg->second->resource.erase(it);
		}
	}

	void TextureManager::disableTexturing(void)
	{
		glActiveTexture(GL_TEXTURE7);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE6);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE5);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE4);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE3);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE2);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE1);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glActiveTexture(GL_TEXTURE0);	glDisable(GL_TEXTURE_2D);	glDisable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}