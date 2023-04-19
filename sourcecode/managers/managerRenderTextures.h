#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "../graphics/renderTexture.h"

namespace Nexus
{
	// A resource group holding resources
	class RenderTextureGroup
	{
	public:
		std::map<std::string, RenderTexture*>	_mmapResource;		// Hash map holding named textures
	};

	// Is responsible for render textures.
	class ManagerRenderTextures : public Singleton<ManagerRenderTextures>
	{
	public:
		ManagerRenderTextures();

		// Return the number of resource groups which currently exist in the manager
		unsigned int getNumGroups(void);

		// Returns the number of resources which currently exist in the named group
		// If the named group doesn't exist, an exception occurs
		unsigned int getNumResInGroup(const std::string& strGroupName);

		// Returns the group name, at specified index
		// If invalid index given, an exception occurs
		const std::string& getGroupName(unsigned int iGroupIndex);

		// Adds a new resource group with the given name which resources can be put into
		// If the named group already exists, an exception occurs.
		void addNewGroup(const std::string& strNewGroupName);

		// Returns true if the named group exists, else false
		bool groupExists(const std::string& strGroupName);

		// Adds a new 2d texture resource to the named group
		// If the group name doesn't exist, an exception occurs.
		// If the resource name already exists, the resource's reference count is increased
		// If the resource doesn't previously exist and it's newly created, it'll be in it's unloaded state
		void addRenderTexture(const std::string& strNewResourceName, int iWidth, int iHeight, const std::string& strGroupName = "default");

		// Returns a pointer to the named resource in it's named group
		// If either the group given doesn't exist, or the named resource doesn't exist, an exception occurs
		RenderTexture* getRenderTexture(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Returns true if the named resource exists in the named group, else false
		bool getExistsRenderTexture(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Removes the named resource from the named group
		// If either the resource or the group that it's in doesn't exist, an exception occurs
		void removeRenderTexture(const std::string& strResourceName, const std::string& strGroupName);

		// Disables texturing
		void disableTexturing(void);
	private:
		std::map<std::string, RenderTextureGroup*> group;	// Hash map holding named resource groups
	};

}