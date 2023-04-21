#pragma once
#include "../../precompiled_header.h"
#include "../../math/vector2.h"
#include "../vertexBuffer.h"

namespace Nexus
{
	// Holds information for a single sprite
	class Sprite
	{
	public:
		Sprite(void);

		void render(const Vector2 &vPositionIn, float fRotationDegrees = 0.0f);
		void setTexture(const std::string& textureNameIn);
		void setDimensions(const Vector2& dimensionsIn);
		Vector2 vPosition;
		Vector2 vDimensions;
	private:
		VertexBuffer vertexBuffer;
		std::string textureName;
		Matrix matrixTransform;
	};


}