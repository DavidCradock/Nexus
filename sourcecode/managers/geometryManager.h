#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "../graphics/geometry.h"

namespace Nexus
{
	// Is responsible for textures.
	class GeometryManager : public Singleton<GeometryManager>
	{
	public:
		GeometryManager();

		// Return the number of resource groups which currently exist in the manager
		unsigned int getNumGroups(void);

		// Returns the number of resources which currently exist in the named group
		// If the named group doesn't exist, an exception occurs
		unsigned int getNumResInGroup(const std::string& strGroupName);

		// Returns the number of resources which are currently loaded in the named group
		// If the named group doesn't exist, an exception occurs
		unsigned int getNumResInGroupLoaded(const std::string& strGroupName);

		// Returns the group name, at specified index
		// If invalid index given, an exception occurs
		const std::string& getGroupName(unsigned int iGroupIndex);

		// Adds a new resource group with the given name which resources can be put into
		// If the named group already exists, this simply returns
		void addNewGroup(const std::string& strNewGroupName);

		// Returns true if the named group exists, else false
		bool groupExists(const std::string& strGroupName);

		// Will load all resources in the named group so that they're ready for use.
		// If the named group doesn't exist, an exception occurs
		// If the group was previously loaded, then some more resources were added, only the newly added resources will be loaded
		// as the previous resources will have already been loaded.
		void loadGroup(const std::string& strGroupName);

		// Will load a single unloaded resource in the named group so that it's ready for use.
		// This will load only a single unloaded resource within the named group, then return.
		// This method is usefull for creating loading screens, where you call this, then call getNumResInGroup and getNumResInGroupLoaded, when both return values from
		// these two methods are the same, all resources are loaded.
		// If the named group doesn't exist, an exception occurs
		// If the group was previously loaded, then some more resources were added, only the newly added resources will be loaded
		// as the previous resources will have already been loaded.
		void loadGroupSingle(const std::string& strGroupName);

		// Will unload all resources in the named group to free up memory.
		// If the named group doesn't exist, an exception occurs
		void unloadGroup(const std::string& strGroupName);

		// Adds a new 2d texture resource to the named group
		// If the group name doesn't exist, an exception occurs.
		// If the resource name already exists, the resource's reference count is increased
		// If the resource doesn't previously exist and it's newly created, it'll be in it's unloaded state
		void add(const std::string& strNewResourceName, const std::string& strGroupName = "default");

		// Returns a pointer to the named resource in it's named group
		// If either the group given doesn't exist, or the named resource doesn't exist, an exception occurs
		// Also, if the resource is in the unloaded state, it is loaded here
		Geometry* get(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Returns true if the named resource exists in the named group, else false
		bool getExists(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Removes the named resource from the named group
		// If either the resource or the group that it's in doesn't exist, an exception occurs
		void remove(const std::string& strResourceName, const std::string& strGroupName);

		// Disables texturing
		void disableTexturing(void);

		// Converts an .obj file to our custom geometry file format and saves to disk
		void convertObj(const std::string filename);
	private:

		// A resource group holding resources
		class Group
		{
		public:
			std::map<std::string, Geometry*> resource;		// Hash map holding named textures
		};

		std::map<std::string, Group*> group;	// Hash map holding named resource groups
	};

}