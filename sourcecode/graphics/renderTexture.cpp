#include "precompiled_header.h"
#include "renderTexture.h"
#include "../core/log.h"
#include "renderDevice.h"

namespace Nexus
{

	RenderTexture::RenderTexture(int iWidth, int iHeight)
	{
		refCount = 0;
		colourTextureID = 0;
		renderBufferID = 0;
		width = iWidth;
		height = iHeight;

		// Create and bind framebuffer object
		glGenFramebuffers(1, &frameBufferObject);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);

		// Create and setup the colour attachment texture
		glGenTextures(1, &colourTextureID);
		glBindTexture(GL_TEXTURE_2D, colourTextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTextureID, 0);

		glGenRenderbuffers(1, &renderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, iWidth, iHeight); // Use a single renderbuffer object for both a depth and stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID); // Attach it
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			Log::getPointer()->addException("RenderTarget() failed. Framebuffer is not complete.");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	RenderTexture::~RenderTexture()
	{
		glDeleteRenderbuffers(1, &renderBufferID);
		glDeleteFramebuffers(1, &frameBufferObject);
		glDeleteTextures(1, &colourTextureID);
	}

	void RenderTexture::bindTexture(void)
	{
		glBindTexture(GL_TEXTURE_2D, colourTextureID);
	}

	void RenderTexture::bindFramebuffer(bool bClear, Vector4 clearColour)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferObject);
		glViewport(0, 0, width, height);
		glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
	}

	void RenderTexture::unbind(void)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		RenderDevice* pRD = RenderDevice::getPointer();
		glViewport(0, 0, pRD->getWindowWidth(), pRD->getWindowHeight());
	}

	unsigned int RenderTexture::getTextureID(void)
	{
		return colourTextureID;
	}

	int RenderTexture::getWidth(void)
	{
		return width;
	}

	int RenderTexture::getHeight(void)
	{
		return height;
	}



	RenderTextureManager::RenderTextureManager()
	{
		addNewGroup("default");
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
			Log::getPointer()->addException(err);
		}
		std::map<std::string, RenderTextureGroup*>::iterator itg = group.find(strGroupName);
		return (unsigned int) itg->second->_mmapResource.size();
	}

	const std::string& RenderTextureManager::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= group.size())
		{
			std::string err("RenderTextureManager::getGroupName(");

			err.append(std::to_string(iGroupIndex));
			err.append(") failed. Invalid index given. Number of groups is ");
			err.append(std::to_string(getNumGroups()));
			Log::getPointer()->addException(err);
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

	void RenderTextureManager::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			std::string err("RenderTextureManager::addNewGroup() has been given the new group name of \"");
			err.append(strNewGroupName);
			err.append("\" but it already exists! Only new groups can be added.");
			Log::getPointer()->addException(err);
		}
		RenderTextureGroup* pNewGroup = new RenderTextureGroup;
		group[strNewGroupName] = pNewGroup;
	}

	bool RenderTextureManager::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, RenderTextureGroup*>::iterator it = group.find(strGroupName);
		if (it == group.end())
			return false;
		return true;
	}

	void RenderTextureManager::addRenderTexture(const std::string& strNewResourceName, int iWidth, int iHeight, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("RenderTextureManager::addRenderTexture(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist!");
			Log::getPointer()->addException(err);
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

	RenderTexture* RenderTextureManager::getRenderTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("RenderTextureManager::getRenderTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist!");
			Log::getPointer()->addException(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, RenderTextureGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, RenderTexture*>::iterator itr = itg->second->_mmapResource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() == itr)
		{
			std::string err("RenderTextureManager::getRenderTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" exists, the named resource couldn't be found!");
			Log::getPointer()->addException(err);
		}
		return (RenderTexture*)itr->second;
	}



	bool RenderTextureManager::getExistsRenderTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, RenderTextureGroup*>::iterator itg = group.find(strGroupName);
		if (itg == group.end())
			return false;
		std::map<std::string, RenderTexture*>::iterator itr = itg->second->_mmapResource.find(strResourceName);
		if (itr == itg->second->_mmapResource.end())
			return false;
		return true;
	}


	void RenderTextureManager::removeRenderTexture(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("RenderTextureManager::removeRenderTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist!");
			Log::getPointer()->addException(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, RenderTextureGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, RenderTexture*>::iterator itr = itg->second->_mmapResource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() == itr)
		{
			std::string err("RenderTextureManager::removeRenderTexture(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, exists, the named resource couldn't be found!");
			Log::getPointer()->addException(err);
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