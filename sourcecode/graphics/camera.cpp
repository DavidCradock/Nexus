#include "precompiled_header.h"
#include "camera.h"



namespace Nexus
{
	Camera::Camera()
	{

	}

	void Camera::setProjection(float fNearClipPlaneDistance, float fFarClipPlaneDistance, float fFOVdegrees, float fTargetWidth, float fTargetHeight)
	{
		projectionMatrix.setProjection(fFOVdegrees, fTargetWidth, fTargetHeight, fNearClipPlaneDistance, fFarClipPlaneDistance);
	}

	void Camera::setView(const Vector3& vEyePosition, const Vector3& vTargetPosition, const Vector3& vUpVector)
	{
		viewMatrix.setView(vEyePosition, vTargetPosition, vUpVector);
	}

	Matrix Camera::getView(void) const
	{
		return viewMatrix;
	}

	Matrix Camera::getProjection(void) const
	{
		return projectionMatrix;
	}

	Matrix Camera::getViewProjection(void)
	{
		return viewProjectionMatrix;
	}
}

