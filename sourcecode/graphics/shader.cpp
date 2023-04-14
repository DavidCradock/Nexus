#include "precompiled_header.h"
#include "shader.h"
#include "../log.h"

namespace Nexus
{
	Shader::Shader(const std::string& vertexShaderFilenameIn, const std::string& fragmentShaderFilenameIn)
	{
		vertexShaderFilename = vertexShaderFilenameIn;
		fragmentShaderFilename = fragmentShaderFilenameIn;
		loaded = false;
		refCount = 0;
		ID = 0;
	}

	void Shader::load(void)
	{
        // Load in source code
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vShaderFile.open(vertexShaderFilename);
            fShaderFile.open(fragmentShaderFilename);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            Log::getPointer()->addException("Shader::compile() failed to read in program code.");
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // Attempt to compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::string err("Shader::compile() failed. ");
            err.append(infoLog);
            Log::getPointer()->addException(err);
        };

        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::string err("Shader::compile() failed. ");
            err.append(infoLog);
            Log::getPointer()->addException(err);
        };

        // Shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // print linking errors if any
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            std::string err("Shader::compile() failed. ");
            err.append(infoLog);
            Log::getPointer()->addException(err);
        }

        // Delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
	}

	void Shader::unload(void)
	{
		glDeleteProgram(ID);
		ID = 0;
	}

    void Shader::use(void)
    {
        glUseProgram(ID);
    }

    void Shader::setBool(const std::string& name, bool value)
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string& name, int value)
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string& name, float value)
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

	void Shader::setMat4(const std::string& name, const glm::mat4x4& matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
		//glm::value_ptr(trans)
	}

	void Shader::setMat4(const std::string& name, const Matrix& matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &matrix.matrix[0]);
	}

	bool Shader::getLoaded(void)
	{
		return loaded;
	}

	ShaderManager::ShaderManager()
	{
		addNewGroup("default");
	}


	unsigned int ShaderManager::getNumGroups(void)
	{
		return (unsigned int)group.size();
	}

	unsigned int ShaderManager::getNumResInGroup(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::getNumResInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			Log::getPointer()->addException(err);
		}
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);
		size_t iTotal = itg->second->_mmapResource.size();
		//	iTotal += itg->second->_mmapResFont.size();
		return (unsigned int)iTotal;
	}

	unsigned int ShaderManager::getNumResInGroupLoaded(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::getNumResInGroupLoaded(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			Log::getPointer()->addException(err);
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

	const std::string& ShaderManager::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= group.size())
		{
			std::string err("ShaderManager::getGroupName(");

			err.append(std::to_string(iGroupIndex));
			err.append(") failed. Invalid index given. Number of groups is ");
			err.append(std::to_string(getNumGroups()));
			Log::getPointer()->addException(err);
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

	void ShaderManager::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			std::string err("ShaderManager::addNewGroup() has been given the new group name of \"");
			err.append(strNewGroupName);
			err.append("\" but it already exists! Only new groups can be added.");
			Log::getPointer()->addException(err);
		}

		ShaderGroup* pNewGroup = new ShaderGroup;
		group[strNewGroupName] = pNewGroup;
	}

	bool ShaderManager::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, ShaderGroup*>::iterator it = group.find(strGroupName);
		if (it == group.end())
			return false;
		return true;
	}

	void ShaderManager::loadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::loadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			Log::getPointer()->addException(err);
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

	void ShaderManager::loadGroupSingle(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::loadGroupSingle(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			Log::getPointer()->addException(err);
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

	void ShaderManager::unloadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::unloadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			Log::getPointer()->addException(err);
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


	void ShaderManager::addShader(const std::string& strNewResourceName, const std::string& strVertexProgram, const std::string& strFragmentProgram, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::addShader(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist!");
			Log::getPointer()->addException(err);
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

	Shader* ShaderManager::getShader(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::getShader(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist!");
			Log::getPointer()->addException(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, Shader*>::iterator itr = itg->second->_mmapResource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() == itr)
		{
			std::string err("ShaderManager::getShader(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" exists, the named resource couldn't be found!");
			Log::getPointer()->addException(err);
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



	bool ShaderManager::getExistsShader(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);
		if (itg == group.end())
			return false;
		std::map<std::string, Shader*>::iterator itr = itg->second->_mmapResource.find(strResourceName);
		if (itr == itg->second->_mmapResource.end())
			return false;
		return true;
	}


	void ShaderManager::removeShader(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("ShaderManager::removeShader(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist!");
			Log::getPointer()->addException(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, ShaderGroup*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, Shader*>::iterator itr = itg->second->_mmapResource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->_mmapResource.end() == itr)
		{
			std::string err("ShaderManager::removeShader(\"");
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