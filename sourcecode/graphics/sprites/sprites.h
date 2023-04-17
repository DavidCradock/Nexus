#pragma once
#include "../../precompiled_header.h"
#include "../../math/vector2.h"
#include "../vertexBuffer.h"
#include "../../core/singleton.h"

namespace Nexus
{
	// Holds information for a single sprite
	class Sprite
	{
	public:
		Sprite(void);

		void render(const Vector2 &vPositionIn);
		void setTexture(const std::string& textureNameIn);
		void setDimensions(const Vector2& dimensionsIn);
		Vector2 vPosition;
		Vector2 vDimensions;
	private:
		VertexBuffer vertexBuffer;
		std::string textureName;
		Matrix matrixTransform;
	};

	// Create all unique sprites and manage them here
	class SpriteManagerTmp : public Singleton<SpriteManagerTmp>
	{
	public:
		// Adds a new sprite
		// If the sprite already exists, an exception occurs
		void addSprite(const std::string& strSpriteName, const std::string& strTextureFilename);

		// Returns a pointer to the named sprite
		// If the named sprite doesn't exist, an exception occurs
		Sprite* getSprite(const std::string& strSpriteName);

		// Returns true if the named sprite exists, else false
		bool getExists(const std::string& strSpriteName);

		// Removes the named sprite
		// If the sprite doesn't exist, an exception occurs
		void removeSprite(const std::string& strSpriteName);
	private:
		std::map<std::string, Sprite*> sprites;	// Hash map holding each sprite
	};
}