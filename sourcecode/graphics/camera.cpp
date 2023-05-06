#include "precompiled_header.h"
#include "camera.h"
#include "renderDevice.h"


namespace Nexus
{
	Camera::Camera()
	{
		RenderDevice* pRenderDevice = RenderDevice::getPointer();
		setProjection(0.1f, 1000.0f, 55.0f, (float)pRenderDevice->getWindowWidth(), (float)pRenderDevice->getWindowHeight());
		setView(Vector3(0.0f, 0.0f, -100.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
		getViewProjection();	// Update the view projection matrix
	}

	void Camera::setProjection(float fNearClipPlaneDistance, float fFarClipPlaneDistance, float fFOVdegrees, float fTargetWidth, float fTargetHeight)
	{
		projectionMatrix.setProjection(fFOVdegrees, fTargetWidth, fTargetHeight, fNearClipPlaneDistance, fFarClipPlaneDistance);
		bViewProjectionMatrixNeedsUpdating = true;
	}

	void Camera::setView(const Vector3& vEyePosition, const Vector3& vTargetPosition, const Vector3& vUpVector)
	{
		viewMatrix.setView(vEyePosition, vTargetPosition, vUpVector);
		bViewProjectionMatrixNeedsUpdating = true;
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
		if (bViewProjectionMatrixNeedsUpdating)
		{
			bViewProjectionMatrixNeedsUpdating = false;
			viewProjectionMatrix = viewMatrix * projectionMatrix;
		}
		return viewProjectionMatrix;
	}
}

