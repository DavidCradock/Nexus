#pragma once
#include "../precompiled_header.h"
#include "../math/vector4.h"
#include "../math/vector3.h"
#include "../math/vector2.h"

namespace Nexus
{
	// A single vertex
	struct Vertex
	{
		Vector3 pos;
		Vector4 colour;
		Vector2 texCoord;
	};

	// Holds vertices for submission to the GPU
	// 
	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		// Clear everything
		void reset(void);

		// Binds the vertexArrayObject and calls glDrawElements()
		void draw(bool bWireframeMode = false);

		// Adds a new vertex, once all indicies and vertices are added, call upload()
		void addVertex(const Vertex& newVertex);

		// Add a new index, once all indicies and vertices have been added, call upload()
		void addIndex(int newIndex);

		// Adds a bunch of vertices and indices which represent a 2D quad
		void addQuad(const Vector2& vPosition, const Vector2& vDimensions, const Vector4& colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f), const Vector2& tcBottomLeft = Vector2(0, 0), const Vector2& tcBottomRight = Vector2(1, 0), const Vector2& tcTopRight = Vector2(1, 1), const Vector2& tcTopLeft = Vector2(0, 1));

		// Adds a line and renders the line as a quad
		void addLine(const Vector2& vLinePoint1, const Vector2& vLinePoint2, float fLineWidth = 1.0f, const Vector4 colour = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		// Once all vertices have been added or loaded, prepares vertex buffer, ready for use.
		void upload(void);

		// Return number of vertices which have been added so far.
		inline size_t getNumVertices(void)
		{
			return vertices.size();
		}

		// Return nummber of indices which have been added so far.
		inline size_t getNumIndicies(void)
		{
			return indices.size();
		}
	private:
		unsigned int vertexBufferObject;
		unsigned int vertexArrayObject;
		unsigned int elementBufferObject;

		std::vector<Vertex> vertices;
		std::vector<int> indices;
	};
}