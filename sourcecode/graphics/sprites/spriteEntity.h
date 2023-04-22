#pragma once
#include "../../precompiled_header.h"
#include "../../math/vector2.h"
#include "../../math/rect2d.h"
#include "../colour.h"
#include "../../core/utils.h"

namespace Nexus
{
	// This holds an instance of a sprite entity.
	//
	// Sprite entities can have a child/parent relationship, where a child entity's rotation and position are concatenated with the parent's.
	// This is usefull for attaching, for example, a child gun entity to a parenting space ship hull.
	// By default, a sprite entity has no parent, IE it's position, rotation and scale are relative to the world.
	// If the sprite entity has a parent, then it's position/rotation/scale is considered local and it's actual world transformation is concatenated with it's parenting sprite entity.
	// To set a parent entity, call setParentEntity().
	class SpriteEntity
	{
		friend class SpriteLayer;
	public:
		SpriteEntity();

		// Set the amount of rotation of the entity in degrees
		inline void setRotationDeg(float fRotationDegressIn) { fRotationRadians = deg2rad(fRotationDegressIn); }

		// Set the amount of rotation of the entity in radians
		inline void setRotationRad(float fRotationRadiansIn) { fRotationRadians = fRotationRadiansIn; }

		// Returns the amount of rotation of the entity in degrees
		inline float getRotationDeg(void) { return rad2deg(fRotationRadians); }

		// Returns the amount of rotation of the entity in radians
		inline float getRotationRad(void) { return fRotationRadians; }

		// Returns the currently set position of the sprite entity (Top left corner of the entity)
		inline Vector2 getPosition(void) { return vPos; }

		// Set the position of the sprite entity (Top left corner of the entity)
		inline void setPosition(const Vector2& vPositionIn) { vPos = vPositionIn; }

		// Set the scale of the sprite (a default of 1.0f is same size as the texture used to render it)
		inline void setScale(float fScaleIn = 1.0f) { fScale = fScaleIn; }

		// Get the scale of the sprite
		float getScale(void) { return fScale; }

		// Set the colour multiplier for the sprite entity (default = white, full opacity)
		inline void setColour(const Colouruc& colourIn) { colour = colourIn; }

		// Returns the currently set colour multiplier for the sprite entity
		inline Colouruc getColour(void) { return colour; }

		// Returns the dimensions of this entity, based on it's currently set scale and the texture size it's using to render itself
		Vector2 getRenderDims(void);

		// Returns the dimensions of the texture (sprite description) which this entity uses
		Vector2 getSpriteDescDims(void);

		// Direct control rect bounds collision detection between this sprite and the one given
		bool getRectBoundsCollision(SpriteEntity& otherSprite);

		// Returns this entity's current animation frame number
		unsigned int getCurrentFrameNumber(void) { return unsigned int(fFrameNumber); }

		// Sets another entity to be this entity's parent so that this entity's transformation are relative to that parent.
		// If another of the parsed names couldn't be found DURING RENDERING, an exception occurs.
		// IE, the parent must exist.
		void setParentEntity(const std::string& strParentEntityLayerNameIn, const std::string& strParentSpriteEntityUniqueNameIn, const std::string& strParentSpriteDescriptionIn);

		// Removes the relationship between this entity and it's parent
		void removeParentEntity(void);

	private:
		Vector2 vPos;					// Position in screen space of this entity
		float fRotationRadians;			// Rotation of entity in degrees
		float fScale;					// The scale multiplier used when rendering the sprite
		Colouruc colour;				// Colour multiplier used when rendering the sprite
		std::string strSpriteDescName;	// The name of the sprite description that this sprite uses to render itself
		float fFrameNumber;				// Although frames are whole numbers, this is used to keep track of current frame number, timebased.

		// For parent entity relationship...
		bool bHasParent;			// For quick checking of whether this entity has a parent
		std::string strParentEntityLayerName;	// The name of this entity's parent's layer
		std::string strParentEntityUniqueName;	// The unique name of this entity's parent's name
		std::string strParentEntitySpriteDesc;	// The unique name of this entity's parent's sprite description
	};


}