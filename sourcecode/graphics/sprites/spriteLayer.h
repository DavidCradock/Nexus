#pragma once
#include "../../precompiled_header.h"
#include "../../math/vector2.h"
#include "spriteEntity.h"
#include "../../math/vector3.h"
#include "../../core/timing.h"

namespace Nexus
{

	// A structure which holds each unique sprite description and any associated sprite entities using that description
	struct SpriteDescAndEnt
	{
		std::map<std::string, SpriteEntity*> _mmapSpriteEntities;	// Hash map of each uniquely named sprite entity using this description
	};

	// Holds an entity's name and the layer it is in (Used for checking collisions between mouse and entities)
	struct SEntityDetails
	{
		std::string strLayerName;				// The layer name which the sprite it stored in
		std::string strEntityName;			// The sprite name
		std::string strEntityDescName;		// The sprite description name that the sprite uses to render itself
	};

	// Sprite entities are placed inside a layer.
	// A layer helps you to render sprites in a specific order
	// A layer renders it's sprite entities in batches, a batch is simply a sprite description
	class SpriteLayer
	{
		friend class ManagerSprites;
	public:
		SpriteLayer();
		~SpriteLayer();

		// Add a new named sprite entity to this layer with parsed values.
		// If the name of the sprite entity already exists, an exception occurs
		// Returns a pointer to the newly added entity
		SpriteEntity* addEntity(const std::string& strSpriteEntityUniqueName, const std::string& strSpriteDescription, Vector2 vSpritePosition = Vector2(0,0), float fSpriteRotationDegrees = 0.0f, float fSpriteScale = 1.0f);

		// Returns true is the named entity exists, else false
		bool entityExists(const std::string& strSpriteEntityUniqueName, const std::string& strSpriteDescription);

		// Returns a pointer to the named sprite entity if it exists, else an exception occurs
		SpriteEntity* getEntity(const std::string& strSpriteEntityUniqueName, const std::string& strSpriteDescription);

		// Removes an entity
		// If the entity couldn't be found, an exception occurs (use getEntity() to make sure it exists)
		void removeEntity(const std::string& strSpriteEntityUniqueName, const std::string& strSpriteDescription);

		// Removes all entities from a layer
		void removeEntityAll(void);

		// Adds a line to be rendered before any sprites to this layer
		// Positions are in world space.
		// These lines are rendered using the line renderer found in render2DLineWS.h/cpp with no texture applied
		void addLine(const Vector2& vLinePos1, const Vector2& vLinePos2, const CColouruc& lineColour1, const CColouruc& lineColour2, float fLineWidth);

		// Removes all lines
		void removeAllLines(void);

		// Adds a circle to be rendered before any sprites to this layer, this circle is rendered with lines
		void addCircle(const Vector2& vCirclePos, float fRadius, const CColouruc& circleColour, float fLineWidth = 0, unsigned int iNumSegments = 18);

		// Removes all circles
		void removeAllCircles(void);

	private:

		std::map<std::string, SpriteDescAndEnt*> _mmapSpriteDescAndSprites;	// Hash map of each uniquely named sprite description type and all of it's sprite entities

		// Renders everything (called by SC2DSpriteManager)
		void _render(const Vector2& vCameraPosition, float fCameraZoom);

		// Determines if this layer has anything to be rendered (called by SC2DSpriteManager)
		bool _hasStuffToRender(void);

		// Used by addLine and removeAllLines
		struct SLineRender
		{
			Vector2 vPos[2];
			CColouruc cCol[2];
			float fLineWidth;
		};
		std::vector<SLineRender*> mvecLines;

		// Used by addCircle and removeAllCircles
		struct SCircleRender
		{
			Vector2 vPos;
			float fRadius;
			CColouruc cCol;
			float fLineWidth;
			unsigned int iNumSegments;
		};
		std::vector<SCircleRender*> mvecCircles;

		Timing _mcTimer;	// Used to update frames for entitys
	};

}