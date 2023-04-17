#include "precompiled_header.h"
#include "texture.h"
#include "../core/log.h"
#include "image.h"

namespace Nexus
{

	Texture::Texture(const std::string& imageFilenameUsed, bool bImageFlipOnLoadIN, TextureFiltering filter)
	{
		bImageFlipOnLoad = bImageFlipOnLoadIN;
		loaded = false;
		refCount = 0;
		imageFilename = imageFilenameUsed;
		textureID = 0;
		imageWidth = imageHeight = 0;
		textureFilter = filter;
	}

	bool Texture::getLoaded(void)
	{
		return loaded;
	}

	void Texture::load(void)
	{
		/*
		Wrapping
		GL_REPEAT: The default behavior for textures. Repeats the texture image.
		GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors the image with each repeat.
		GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
		GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a user-specified border color.

		Border colour
		float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


*/
		Image image;
		image.load(imageFilename, bImageFlipOnLoad);
		glGenTextures(1, &textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		// Set the texture wrapping/filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (textureFilter == TextureFiltering::mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (textureFilter == TextureFiltering::linear)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
		}
		else if (textureFilter == TextureFiltering::nearest)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
		}
		imageWidth = image.getWidth();
		imageHeight = image.getHeight();
	}

	void Texture::unload(void)
	{

	}

	void Texture::bind(void)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	unsigned int Texture::getID(void)
	{
		return textureID;
	}

	int Texture::getWidth(void)
	{
		if (0 == imageWidth)
		{
			int numChan;
			Image img;
			img.loadInfo(imageFilename, imageWidth, imageHeight, numChan);
		}
		return imageWidth;
	}

	int Texture::getHeight(void)
	{
		if (0 == imageHeight)
		{
			int numChan;
			Image img;
			img.loadInfo(imageFilename, imageWidth, imageHeight, numChan);
		}
		return imageHeight;
	}



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
			throw std::runtime_error(err);
		}
		std::map<std::string, TextureGroup*>::iterator itg = group.find(strGroupName);
		size_t iTotal = itg->second->_mmapResource.size();
		//	iTotal += itg->second->_mmapResFont.size();
		return (unsigned int)iTotal;
	}

	unsigned int TextureManager::getNumResInGroupLoaded(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::getNumResInGroupLoaded(\"");
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

	const std::string& TextureManager::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= group.size())
		{
			std::string err("TextureManager::getGroupName(");

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

	void TextureManager::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			std::string err("TextureManager::addNewGroup() has been given the new group name of \"");
			err.append(strNewGroupName);
			err.append("\" but it already exists! Only new groups can be added.");
			throw std::runtime_error(err);
		}

		TextureGroup* pNewGroup = new TextureGroup;
		group[strNewGroupName] = pNewGroup;
	}

	bool TextureManager::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, TextureGroup*>::iterator it = group.find(strGroupName);
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

	void TextureManager::loadGroupSingle(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::loadGroupSingle(\"");
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

	void TextureManager::unloadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::unloadGroup(\"");
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


	void TextureManager::add2DTexture(const std::string& strNewResourceName, const std::string& strTextureFilename, const std::string& strGroupName, bool bImageFlipOnLoad, TextureFiltering filter)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::add2DTexture(\"");
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
			// Increase reference count of the resource
			itr->second->refCount++;
			return;
		}

		// If we get here, we have got to create, then add the resource to the existing named group
		Texture* pNewRes = new Texture(strTextureFilename, bImageFlipOnLoad, filter);
		itg->second->_mmapResource[strNewResourceName] = pNewRes;
	}

	Texture* TextureManager::get2DTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::get2DTexture(\"");
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
			std::string err("TextureManager::get2DTexture(\"");
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



	bool TextureManager::getExists2DTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, TextureGroup*>::iterator itg = group.find(strGroupName);
		if (itg == group.end())
			return false;
		std::map<std::string, Texture*>::iterator itr = itg->second->_mmapResource.find(strResourceName);
		if (itr == itg->second->_mmapResource.end())
			return false;
		return true;
	}


	void TextureManager::remove2DTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("TextureManager::remove2DTexture(\"");
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
			std::string err("TextureManager::remove2DTexture(\"");
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
	}
}