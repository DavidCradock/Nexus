#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "../math/vector4.h"

namespace Nexus
{
	// A single render texture
	class RenderTexture
	{
		friend class RenderTextureManager;

	public:
		RenderTexture(int iWidth, int iHeight);

		~RenderTexture();

		// Binds the texture, so it may be used as a normal texture
		void bindTexture(void);

		// Binds the framebuffer object, so all future rendering will be into the framebuffer
		void bindFramebuffer(bool bClear = true, Vector4 clearColour = Vector4(0.0f, 0.0f, 0.0f, 1.0f));

		// Unbinds both the texture and the framebuffer object
		void unbind(void);

		// Returns the OpenGL texture ID
		unsigned int getTextureID(void);

		// Returns width of image used for the texture.
		int getWidth(void);

		// Returns width of image used for the texture.
		int getHeight(void);
	private:
		unsigned int refCount;			// Used by the manager 
		unsigned int colourTextureID;	// OpenGL ID for colour
		unsigned int renderBufferID;	// OpenGL ID for render buffer
		unsigned int frameBufferObject;	
		int width;
		int height;
	};

	// A resource group holding resources
	class RenderTextureGroup
	{
	public:
		std::map<std::string, RenderTexture*>	_mmapResource;		// Hash map holding named textures
	};

	// Is responsible for render textures.
	class RenderTextureManager : public Singleton<RenderTextureManager>
	{
	public:
		RenderTextureManager();

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