#include "precompiled_header.h"
#include "managerShaders.h"
#include "../core/log.h"

namespace Nexus
{
	ManagerShaders::ManagerShaders()
	{
	}

	unsigned int ManagerShaders::getNumGroups(void)
	{
		return (unsigned int)group.size();
	}

	unsigned int ManagerShaders::getNumResInGroup(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerShaders::getNumResInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			throw std::runtime_error(err);
		}
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);
		size_t iTotal = itg->second->_mmapResource.size();
		//	iTotal += itg->second->_mmapResFont.size();
		return (unsigned int)iTotal;
	}

	unsigned int ManagerShaders::getNumResInGroupLoaded(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerShaders::getNumResInGroupLoaded(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			throw std::runtime_error(err);
		}
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);

		unsigned int iResLoadedTotal = 0;
		std::map<std::string, Shader*>::iterator it0 = itg->second->_mmapResource.begin();
		while (it0 != itg->second->_mmapResource.end())
		{
			if (it0->second->getLoaded())
				++iResLoadedTotal;
			it0++;
		}

		return iResLoadedTotal;
	}

	const std::string& ManagerShaders::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= group.size())
		{
			std::string err("ManagerShaders::getGroupName(");

			err.append(std::to_string(iGroupIndex));
			err.append(") failed. Invalid index given. Number of groups is ");
			err.append(std::to_string(getNumGroups()));
			throw std::runtime_error(err);
		}
		std::map<std::string, ShaderGroup*>::iterator itg = group.begin();
		unsigned int i = 0;
		while (i < iGroupIndex)
		{
			itg++;
			++i;
		}
		return itg->first;
	}

	void ManagerShaders::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			std::string err("ManagerShaders::addNewGroup() has been given the new group name of \"");
			err.append(strNewGroupName);
			err.append("\" but it already exists! Only new groups can be added.");
			throw std::runtime_error(err);
		}

		ShaderGroup* pNewGroup = new ShaderGroup;
		group[strNewGroupName] = pNewGroup;
	}

	bool ManagerShaders::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, ShaderGroup*>::iterator it = group.find(strGroupName);
		if (it == group.end())
			return false;
		return true;
	}

	void ManagerShaders::loadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerShaders::loadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			throw std::runtime_error(err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);

		// For each 2d texture resource in this group...
		std::map<std::string, Shader*>::iterator itShader = itg->second->_mmapResource.begin();
		while (itShader != itg->second->_mmapResource.end())
		{
			if (!itShader->second->loaded)
			{
				itShader->second->load();
				itShader->second->loaded = true;
			}
			itShader++;
		}

	}

	void ManagerShaders::loadGroupSingle(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerShaders::loadGroupSingle(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			throw std::runtime_error(err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);


		// For each resource in this group...
		std::map<std::string, Shader*>::iterator itShader = itg->second->_mmapResource.begin();
		while (itShader != itg->second->_mmapResource.end())
		{
			if (!itShader->second->loaded)
			{
				itShader->second->load();
				itShader->second->loaded = true;
				return;	// We've changed a resource from unloaded to loaded state, aka, we've loaded a resource, simply return.
			}
			itShader++;
		}

	}

	void ManagerShaders::unloadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerShaders::unloadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			throw std::runtime_error(err);
		}

		// Unload any loaded resources within the group
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);

		// For each 2d texture resource in this group...
		std::map<std::string, Shader*>::iterator itShader = itg->second->_mmapResource.begin();
		while (itShader != itg->second->_mmapResource.end())
		{
			if (itShader->second->loaded)
			{
				itShader->second->unload();
				itShader->second->loaded = false;
			}
			itShader++;
		}
	}


	void ManagerShaders::addShader(const std::string& strNewResourceName, const std::string& strVertexProgram, const std::string& strFragmentProgram, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerShaders::addShader(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist!");
			throw std::runtime_error(err);
		}

		// Resource already exists in the group?
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, Shader*>::iterator itr = itg->second->_mmapResource.find(strNewResourceName);	// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() != itr)
		{
			// Increase reference count of the resource
			itr->second->refCount++;
			return;
		}

		// If we get here, we have got to create, then add the resource to the existing named group
		Shader* pNewRes = new Shader(strVertexProgram, strFragmentProgram);
		itg->second->_mmapResource[strNewResourceName] = pNewRes;
	}

	Shader* ManagerShaders::getShader(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerShaders::getShader(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist!");
			throw std::runtime_error(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, Shader*>::iterator itr = itg->second->_mmapResource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() == itr)
		{
			std::string err("ManagerShaders::getShader(\"");
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
		return (Shader*)itr->second;
	}



	bool ManagerShaders::getExistsShader(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);
		if (itg == group.end())
			return false;
		std::map<std::string, Shader*>::iterator itr = itg->second->_mmapResource.find(strResourceName);
		if (itr == itg->second->_mmapResource.end())
			return false;
		return true;
	}


	void ManagerShaders::removeShader(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ManagerShaders::removeShader(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist!");
			throw std::runtime_error(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, Shader*>::iterator itr = itg->second->_mmapResource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() == itr)
		{
			std::string err("ManagerShaders::removeShader(\"");
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
}