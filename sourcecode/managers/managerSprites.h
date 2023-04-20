#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "../core/timing.h"
#include "../graphics/sprites/spriteDescription.h"
#include "../graphics/sprites/spriteEntity.h"
#include "../graphics/sprites/spriteLayer.h"
#include "../graphics/sprites/sprites.h"
#include "../core/utils.h"

namespace Nexus
{

	// Holds an entity's name, the layer it is in, the description's name it is using, it's dimensions and position (Used for checking collisions between sprite entities)
	struct SEntityDetailsEx
	{
		std::string strLayerName;			// The layer name which the sprite it stored in
		std::string strEntityName;			// The sprite name
		std::string strEntityDescName;		// The sprite description name that the sprite uses to render itself
		Vector2 vSpriteDims;				// The sprite's dimensions
		Vector2 vSpritePos;					// The sprite's position
	};

	struct SEntityCollisionPairs		// Holds sprite entity details for a pair of colliding sprite entities
	{
		SEntityDetailsEx spriteEntity1;
		SEntityDetailsEx spriteEntity2;
	};

	// This is the sprite manager
	// It renders 2D sprites in world space
	// You use this to create sprite descriptions which define their appearence, then you create one or more layers. Then you
	// add sprite entities using a layer, parsing a sprite description.
	class ManagerSprites : public Singleton<ManagerSprites>
	{
	public:

		// Constructor, one time init
		ManagerSprites();

		// Add a new named layer which will contain sprite entities
		// When adding a new layer, it will be placed "above" all previously added layers, making it's sprite entities rendered last aka above all other layers.
		// If the layer name already exists, an exception occurs
		// Returns a pointer to the newly added layer
		SpriteLayer* addLayer(const std::string& strUniqueName);

		// Returns true if a layer exists, else false
		bool layerExists(const std::string& strUniqueName);

		// Returns a pointer to a previously added named layer (with addLayer()) if it exists, else an exception occurs
		SpriteLayer* getLayer(const std::string& strUniqueName);

		// Removes a layer (and all the entities which have been added to it)
		// If the layer's name given doesn't exist, an exception occurs
		void removeLayer(const std::string& strUniqueName);

		// Removes all layers (and all the entities which have been added to them)
		void removeAllLayers(void);

		// Resets camera to default setting
		void resetCamera(void);

		// Returns the total number of added layers
		int getLayerCount(void) { return int(_mvecLayerNameZOrder.size()); }

		// Returns the name of the layer at specified z order
		// A z order of 0 would give you the back most layer
		// Use getLayerCount() to get the number of layers
		// If invalid z order given, an exception occurs
		std::string getLayerName(int iZorder);

		// Given a name of an existing layer, returns it's order of rendering
		// If it's at the back, this would be 0
		// If it's at the front, this would be the total number of added layers - 1
		// If the layer couldn't be found, an exception occurs
		int getLayerZorder(const std::string& strLayerName);

		// Given a name of an existing layer (strLayerName), moves it closer to the front by one
		// If the layer name doesn't exist, an exception occurs
		void moveLayerToFrontByOne(const std::string& strLayerName);

		// Given a name of an existing layer (strLayerName), moves it closer to the back by one
		// If the layer name doesn't exist, an exception occurs
		void moveLayerToBackByOne(const std::string& strLayerName);

		// Given a name of an existing layer (strLayerName), moves it completely to the back
		// If the layer name doesn't exist, an exception occurs
		void moveLayerToBack(const std::string& strLayerName);

		// Given a name of an existing layer (strLayerName), moves it completely to the front
		// If the layer name doesn't exist, an exception occurs
		void moveLayerToFront(const std::string& strLayerName);

		// Given a name of an existing layer (strLayerName) and another existing layer (strLayerNameOther), moves strLayerName so that it is behind strLayerNameOther
		// If either of the the layer names don't exist, an exception occurs
		void moveLayerBehind(const std::string& strLayerName, const std::string& strLayerNameOther);

		// Given a name of an existing layer (strLayerName) and another existing layer (strLayerNameOther), moves strLayerName so that it is in front of strLayerNameOther
		// If either of the the layer names don't exist, an exception occurs
		void moveLayerInfront(const std::string& strLayerName, const std::string& strLayerNameOther);

		// Add a new named sprite description
		// If the description name already exists, an exception occurs
		// Returns a pointer to the newly created description
		SpriteDescription* addDescription(const std::string& strUniqueName);

		// Returns a pointer to a previously added named sprite description (with addDescription()) if it exists, else NULL is returned
		SpriteDescription* getDescription(const std::string& strUniqueName);

		// Removes all previously added sprite descriptions, all layers and sprites.
		void removeAll(void);

		// Sets the camera position
		inline void setCameraPosition(const Vector2& vCameraPosition) { _mvCameraPositionCurrent = vCameraPosition;	_mvCameraPositionTarget = vCameraPosition; }

		// Moves the camera from it's current position to the new specified position
		void setCameraPosition(const Vector2& vCameraTargetPosition, float fSpeed);

		// Returns camera's current position
		inline Vector2 getCameraPosition(void) { return _mvCameraPositionCurrent; }

		// Sets the camera zoom
		// You can think of the zoom level as distance from the scene along z axis.
		// This value is clamped between to prevent "inverted projection" to 0.0001f and FLT_MAX
		// 1.0f = no zoom in or out
		// 0.00f = really, really zoomed in!
		inline void setCameraZoom(float fZoom) { clamp(fZoom, 0.0001f, FLT_MAX);	_mfCameraZoomCurrent = fZoom;	_mfCameraZoomTarget = fZoom; }

		// Returns the camera's current zoom
		inline float getCameraZoom(void) { return _mfCameraZoomCurrent; }

		// Sets the camera zoom level which will be moved towards over time
		// You can think of the zoom level as distance from the scene along z axis.
		// This value is clamped between to prevent "inverted projection" to 0.0001f and FLT_MAX
		// 1.0f = no zoom in or out
		// 0.00f = really, really zoomed in!
		// The higher the value of fZoomSpeed, the quicker the camera will move towards the target value
		inline void setCameraZoom(float fZoomTarget, float fZoomSpeed) { _mfCameraZoomTarget = fZoomTarget;	_mfCameraZoomSpeed = fZoomSpeed; }

		// If setCameraZoom has been called, this will returns the camera's currently set zoom target
		inline float getCameraZoomTarget(void) { return _mfCameraZoomTarget; }

		// Using the currently set camera position and zoom, this will take a screen space position and compute it's
		// position in world space and return the result
//		CVector2 convertScreenspaceToWorldspace(const CVector2& vScreenSpacePosition);

		// Using the currently set camera position and zoom, this will take a world space position and compute it's
		// position in screen space and return the result
//		CVector2 convertWorldspaceToScreenspace(const CVector2& vWorldSpacePosition);
//	private:
		
		// Called from main
		void update(void);

		// Called from main
		void render(void);

		// Returns whether there is anything which needs rendering
		bool hasStuffToRender(void);

		std::map<std::string, SpriteLayer*> _mmapLayers;				// Each named layer which holds sprite entities
		std::map<std::string, SpriteDescription*> _mmapDescriptions;	// Each named sprite description

		std::vector<std::string> _mvecLayerNameZOrder;		// Holds names of each layer, in their rendering order (first added = first rendered)

		Timing _mcTimer;					// Timer object 
		Vector2 _mvCameraPositionCurrent;	// This is the camera's current position
		Vector2 _mvCameraPositionTarget;	// This is the camera's target position
		float _mfCameraPositionSpeed;		// This is used to move _mvCameraPositionCurrent towards _mvCameraPositionTarget over time
		float _mfCameraZoomCurrent;			// Used by this class's setCameraZoom???? methods. This is the actual current camera zoom level (The level used to build the projection matrix)
		float _mfCameraZoomTarget;			// Used by this class's setCameraZoom???? methods. This is the target zoom level, we move the current camera zoom level towards this value over time.
		float _mfCameraZoomSpeed;			// Used to move _mfCameraZoomCurrent towards _mfCameraZoomTarget over time
	};

}