#pragma once
#include "../../precompiled_header.h"
#include "../../math/vector2.h"
#include "../../math/vector3.h"

namespace Nexus
{
	// A single geometry object class
	class Geometry
	{
		friend class GeometryManager;

	public:
		Geometry(std::string strFilename);
		bool getLoaded(void);
		void load(void);
		void unload(void);

		void draw(bool bWireframeMode);
	private:
		bool loaded;			// Accessed by manager
		unsigned int refCount;	// Accessed by manager

		struct Vertex
		{
			Vector3 pos;
			Vector3 normal;
			Vector2 texCoord;
		};
		std::string filename;
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		unsigned int vertexBufferObject;
		unsigned int vertexArrayObject;
		unsigned int elementBufferObject;
	};

}