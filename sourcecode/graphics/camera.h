#pragma once
#include "../precompiled_header.h"
#include "../math/matrix.h"
#include "../math/vector3.h"

namespace Nexus
{
	// 3D camera
	class Camera
	{
	public:
		Camera();

		// Set settings for this camera's projection matrix
		void setProjection(float fNearClipPlaneDistance, float fFarClipPlaneDistance, float fFOVdegrees, float fTargetWidth, float fTargetHeight);

		// Set settings for this camera's view matrix
		// This is used to transform the view point (aka, the camera)
		void setView(const Vector3& vEyePosition, const Vector3& vTargetPosition, const Vector3 &vUpVector = Vector3(0, 1, 0));

		// Returns a pointer to the view matrix
		Matrix getView(void) const;

		// Returns a pointer to the projection matrix
		Matrix getProjection(void) const;

		// Returns a pointer to the combined view and projection matrix
		Matrix getViewProjection(void);

	private:
		Matrix viewMatrix;
		Matrix projectionMatrix;
		Matrix viewProjectionMatrix;	// Both view and projection combined
		bool bViewProjectionMatrixNeedsUpdating;
	};
}