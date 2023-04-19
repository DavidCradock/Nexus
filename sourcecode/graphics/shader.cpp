#include "precompiled_header.h"
#include "shader.h"
#include "../core/log.h"

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
			throw std::runtime_error("Shader::compile() failed to read in program code.");
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
			throw std::runtime_error(err);
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
			throw std::runtime_error(err);
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
			throw std::runtime_error(err);
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
}