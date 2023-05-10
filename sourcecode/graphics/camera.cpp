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
		vViewEyePosition = vEyePosition;
		vViewTargetPosition = vTargetPosition;
		vViewUpVector = vUpVector;
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
			viewProjectionMatrix = projectionMatrix * viewMatrix;
		}
		return viewProjectionMatrix;
	}

	void Camera::moveLocal(float fForward, float fRight, float fUp, bool bMoveTargetToo)
	{
		Vector3 vecForward;
		Vector3 vecRight;
		Vector3 vecUp;
		Vector3 vecTranslation;

		viewMatrix.getForwardVector(vecForward);
		viewMatrix.getRightVector(vecRight);
		viewMatrix.getUpVector(vecUp);
		viewMatrix.getTranslation(vecTranslation);

		// Multiply vectors by given values to compute amount to move
		vecForward.multiply(fForward);
		vecRight.multiply(fRight);
		vecUp.multiply(fUp);

		// Compute new position
		vViewEyePosition += vecForward;
		vViewEyePosition += vecRight;
		vViewEyePosition += vecUp;

		if (bMoveTargetToo)
		{
			vViewTargetPosition += vecForward;
			vViewTargetPosition += vecRight;
			vViewTargetPosition += vecUp;

		}

		// Recalculate view matrix
		viewMatrix.setView(vViewEyePosition, vViewTargetPosition, vViewUpVector);
		
		// Recalculate combined view and project matrices...
//		viewProjectionMatrix = projectionMatrix * viewMatrix;
		viewProjectionMatrix = viewMatrix * projectionMatrix;
	}

	void Camera::moveWorld(float fX, float fY, float fZ, bool bMoveTargetToo)
	{

	}
}

