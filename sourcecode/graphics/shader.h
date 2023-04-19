#pragma once
#include "../precompiled_header.h"
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
		friend class ManagerShaders;
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
}