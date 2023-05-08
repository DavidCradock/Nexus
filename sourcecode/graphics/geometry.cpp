#include "precompiled_header.h"
#include "geometry.h"
#include "../core/log.h"

namespace Nexus
{
	Geometry::Geometry(std::string strFilename)
	{
		filename = strFilename;
		loaded = false;
		refCount = 0;

		vertexBufferObject = 0;
		vertexArrayObject = 0;
		elementBufferObject = 0;
		glGenVertexArrays(1, &vertexArrayObject);
		glGenBuffers(1, &vertexBufferObject);
		glGenBuffers(1, &elementBufferObject);
	}

	bool Geometry::getLoaded(void)
	{
		return loaded;
	}

	void Geometry::load(void)
	{
		FILE* file = 0;
		errno_t err;
		err = fopen_s(&file, filename.c_str(), "r+b");
		if (err != 0)
		{
			std::string strErr("Geometry::load() failed to open file ");
			strErr.append(filename);
			Log::getPointer()->exception(strErr);
		}

		// Loop through each face, generating and loading as we go
		Vertex geomVertex;
		unsigned int iNumVertices = 0;
		size_t bytesRead = fread(&iNumVertices, sizeof(unsigned int), 1, file);
		if (0 == bytesRead)
		{
			std::string strErr("Geometry::load() failed whilst reading file ");
			strErr.append(filename);
			Log::getPointer()->exception(strErr);
		}
		for (unsigned int i = 0; i < iNumVertices; ++i)
		{
			bytesRead = fread(&geomVertex, sizeof(Vertex), 1, file);
			if (0 == bytesRead)
			{
				std::string strErr("Geometry::load() failed whilst reading file ");
				strErr.append(filename);
				Log::getPointer()->exception(strErr);
			}
			vertices.push_back(geomVertex);
			indices.push_back(i);
		}
		fclose(file);

		glBindVertexArray(vertexArrayObject);

		// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
		// GL_STATIC_DRAW: the data is set only once and used many times.
		// GL_DYNAMIC_DRAW : the data is changed a lot and used many times.

		// Bind VBO and upload vertex data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// Indicies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0,			// position in vertex programs
			3,								// Number of components
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			(void*)0);
		glEnableVertexAttribArray(0);

		// Normals
		glVertexAttribPointer(1,			// Position in vertex programs
			3,								// Number of components
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			(void*)(3 * sizeof(float)));	// Offset
		glEnableVertexAttribArray(1);

		// Texture coordinates
		glVertexAttribPointer(2,			// Position in vertex programs
			2,								// Number of components
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			(void*)(6 * sizeof(float)));	// Offset
		glEnableVertexAttribArray(2);

		// Unbind stuff as we're done
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		fclose(file);
	}

	void Geometry::unload(void)
	{
		glDeleteVertexArrays(1, &vertexArrayObject);
		glDeleteBuffers(1, &vertexBufferObject);
		glDeleteBuffers(1, &elementBufferObject);
		vertexBufferObject = 0;
		vertexArrayObject = 0;
		elementBufferObject = 0;
		vertices.clear();
		indices.clear();
	}

	void Geometry::draw(bool bWireframeMode)
	{
		if (bWireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindVertexArray(vertexArrayObject);
		glDrawElements(
			GL_TRIANGLES,				// Mode
			(GLsizei)indices.size(),	// Count
			GL_UNSIGNED_INT,
			0);
		glBindVertexArray(0);
	}
}