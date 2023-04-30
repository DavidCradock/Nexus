#include "precompiled_header.h"
#include "vertexBuffer.h"
#include "../core/log.h"

namespace Nexus
{

	VertexBuffer::VertexBuffer()
	{
		vertexBufferObject = 0;
		vertexArrayObject = 0;
		elementBufferObject = 0;
		glGenVertexArrays(1, &vertexArrayObject);
		glGenBuffers(1, &vertexBufferObject);
		glGenBuffers(1, &elementBufferObject);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteVertexArrays(1, &vertexArrayObject);
		glDeleteBuffers(1, &vertexBufferObject);
		glDeleteBuffers(1, &elementBufferObject);
	}

	void VertexBuffer::reset(void)
	{
		vertices.clear();
		indices.clear();
	}

	void VertexBuffer::addVertex(const Vertex& newVertex)
	{
		vertices.push_back(newVertex);
	}

	void VertexBuffer::addIndex(int newIndex)
	{
		indices.push_back(newIndex);
	}

	void VertexBuffer::addQuad(const Vector2& vPosition, const Vector2& vDimensions, const Vector4& colour, const Vector2 &tcBottomLeft, const Vector2 &tcBottomRight, const Vector2 &tcTopRight, const Vector2 &tcTopLeft)
	{
		// Build a simple quad
		Vertex vertex;

		// Indicies
		unsigned int iIndex = (unsigned int)vertices.size();
		addIndex(iIndex);		// BL
		addIndex(iIndex + 1);	// TL
		addIndex(iIndex + 2);	// TR

		addIndex(iIndex + 2);	// TR
		addIndex(iIndex + 3);	// BR
		addIndex(iIndex);		// BL

		// Bottom left
		vertex.pos.x = vPosition.x;
		vertex.pos.y = vPosition.y + vDimensions.y;
		vertex.pos.z = 0.0f;
		vertex.colour = colour;
		vertex.texCoord = tcBottomLeft;
		addVertex(vertex);

		// Top left
		vertex.pos.y = vPosition.y;
		vertex.texCoord = tcTopLeft;
		addVertex(vertex);

		// Top right
		vertex.pos.x = vPosition.x + vDimensions.x;
		vertex.texCoord = tcTopRight;
		addVertex(vertex);

		// Bottom right
		vertex.pos.y = vPosition.y + vDimensions.y;
		vertex.texCoord = tcBottomRight;
		addVertex(vertex);		
	}

	void VertexBuffer::upload(void)
	{
		if (!vertices.size())
			return;
		if (!indices.size())
			return;

		glBindVertexArray(vertexArrayObject);

		// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
		// GL_STATIC_DRAW: the data is set only once and used many times.
		// GL_DYNAMIC_DRAW : the data is changed a lot and used many times.

		// Bind VBO and upload vertex data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// Indicies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0,			// position in vertex programs
			3,								// Number of components
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),					// 8 * sizeof(float) Stride (3 for pos, 4 for colour and 2 for texture coordinates)
			(void*)0);
		glEnableVertexAttribArray(0);

		// Colour
		glVertexAttribPointer(1,			// Position in vertex programs
			3,								// Number of components
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),					// Stride (3 for pos, 4 for colour and 2 for texture coordinates)
			(void*)(3 * sizeof(float)));	// Offset
		glEnableVertexAttribArray(1);

		// Texture coordinates
		glVertexAttribPointer(2,			// Position in vertex programs
			2,								// Number of components
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),					// Stride (3 for pos, 4 for colour and 2 for texture coordinates)
			(void*)(7 * sizeof(float)));	// Offset
		glEnableVertexAttribArray(2);

		// Unbind stuff as we're done
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::draw(bool bWireframeMode)
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