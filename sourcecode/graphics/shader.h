#pragma once
#include "../precompiled_header.h"
#include "../singleton.h"
#include "../math/matrix.h"

namespace Nexus
{
	// Holds a pair of vertex and fragment shaders
	// Example usage:
	// Shader *pShader = ShaderManager::getPointer()->getShader("shaderName");
	// Texture *pTexture = TextureManager::getPointer()->getTexture("textureName");
	// pShader->setInt("texture1", pTexture->getID());
	// Matrix matrixOrtho;
	// matrixOrtho.setOrthographic();
	// pShader->setMat4("transform", matrixOrtho);
	// pShader->use();
	class Shader
	{
		friend class ShaderManager;
	public:
		// Constructor to store filenames of the vertex and fragment programs
		Shader(const std::string& vertexShaderFilenameIn, const std::string& fragmentShaderFilenameIn);

		// Attempts to load and compile the shader programs
		// If an error occurs, an exception is thrown
		void load(void);

		// Unloads the thing
		void unload(void);

		// Bind the shader
		void use(void);

		// Set uniform
		void setBool(const std::string& name, bool value);
		
		// Set uniform
		void setInt(const std::string& name, int value);
		
		// Set uniform
		void setFloat(const std::string& name, float value);
		
		// Set named matrix
		void setMat4(const std::string& name, const glm::mat4x4 &matrix);

		// Set named matrix
		void setMat4(const std::string& name, const Matrix& matrix);

		bool getLoaded(void);
	private:
		std::string vertexShaderFilename;
		std::string fragmentShaderFilename;
		unsigned int ID;
		bool loaded;
		unsigned int refCount;		// Used by the manager 
	};

	// A resource group holding resources
	class ShaderGroup
	{
	public:
		std::map<std::string, Shader*>	_mmapResource;		// Hash map holding named shaders
	};

	class ShaderManager : public Singleton<ShaderManager>
	{
	public:
		ShaderManager();

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
		// If the named group already exists, an exception occurs.
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

		// Adds a new shader resource to the named group
		// If the group name doesn't exist, an exception occurs.
		// If the resource name already exists, the resource's reference count is increased
		// If the resource doesn't previously exist and it's newly created, it'll be in it's unloaded state
		void addShader(const std::string& strNewResourceName, const std::string& strVertexProgram, const std::string& strFragmentProgram, const std::string& strGroupName = "default");

		// Returns a pointer to the named resource in it's named group
		// If either the group given doesn't exist, or the named resource doesn't exist, an exception occurs
		// Also, if the resource is in the unloaded state, it is loaded here
		Shader* getShader(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Returns true if the named resource exists in the named group, else false
		bool getExistsShader(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Removes the named resource from the named group
			// If either the resource or the group that it's in doesn't exist, an exception occurs
		void removeShader(const std::string& strResourceName, const std::string& strGroupName);

	private:
		std::map<std::string, ShaderGroup*> group;	// Hash map holding named resource groups
	};
}