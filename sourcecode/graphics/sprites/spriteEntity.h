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
		inline void setRotationDeg(float fRotationDegress) { mfRotationRadians = deg2rad(fRotationDegress); }

		// Set the amount of rotation of the entity in radians
		inline void setRotationRad(float fRotationRadians) { mfRotationRadians = fRotationRadians; }

		// Returns the amount of rotation of the entity in degrees
		inline float getRotationDeg(void) { return rad2deg(mfRotationRadians); }

		// Returns the amount of rotation of the entity in radians
		inline float getRotationRad(void) { return mfRotationRadians; }

		// Returns the currently set position of the sprite entity (Top left corner of the entity)
		inline Vector2 getPosition(void) { return mvPos; }

		// Set the position of the sprite entity (Top left corner of the entity)
		inline void setPosition(const Vector2& vPosition) { mvPos = vPosition; }

		// Set the scale of the sprite (a default of 1.0f is same size as the texture used to render it)
		inline void setScale(float fScale = 1.0f) { mfScale = fScale; }

		// Get the scale of the sprite
		float getScale(void) { return mfScale; }

		// Set the colour multiplier for the sprite entity (default = white, full opacity)
		inline void setColour(const CColouruc& colour) { mvColour = colour; }

		// Returns the currently set colour multiplier for the sprite entity
		inline CColouruc getColour(void) { return mvColour; }

		// Returns the dimensions of this entity, based on it's currently set scale and the texture size it's using to render itself
		Vector2 getRenderDims(void);

		// Returns the dimensions of the texture (sprite description) which this entity uses
		Vector2 getSpriteDescDims(void);

		// Direct control rect bounds collision detection between this sprite and the one given
		bool getRectBoundsCollision(SpriteEntity& otherSprite);

		// Returns this entity's current animation frame number
		unsigned int getCurrentFrameNumber(void) { return unsigned int(mfFrameNumber); }

		// Sets another entity to be this entity's parent so that this entity's transformation are relative to that parent.
		// If another of the parsed names couldn't be found DURING RENDERING, an exception occurs.
		// IE, the parent must exist.
		void setParentEntity(const std::string& strParentEntityLayerName, const std::string& strParentSpriteEntityUniqueName, const std::string& strParentSpriteDescription);

		// Removes the relationship between this entity and it's parent
		void removeParentEntity(void);

	private:
		Vector2 mvPos;				// Position in screen space of this entity
		float mfRotationRadians;	// Rotation of entity in degrees
		float mfScale;				// The scale multiplier used when rendering the sprite
		CColouruc mvColour;			// Colour multiplier used when rendering the sprite
		std::string mStrSpriteDescName;	// The name of the sprite description that this sprite uses to render itself
		float mfFrameNumber;		// Although frames are whole numbers, this is used to keep track of current frame number, timebased.

		// For parent entity relationship...
		bool mbHasParent;			// For quick checking of whether this entity has a parent
		std::string mstrParentEntityLayerName;	// The name of this entity's parent's layer
		std::string mstrParentEntityUniqueName;	// The unique name of this entity's parent's name
		std::string mstrParentEntitySpriteDesc;	// The unique name of this entity's parent's sprite description
	};


}