#pragma once
#include "../precompiled_header.h"
#include "../graphics/renderDevice.h"


namespace Nexus
{
	class Vector3;

	// 4x4 OpenGL matrix class
	//
	class Matrix
	{
		friend class Quaternion;
		friend class Vector3;
	public:
		// Default constructor, sets matrix as identity matrix
		Matrix();

		Matrix(float f11, float f12, float f13, float f14,
			float f21, float f22, float f23, float f24,
			float f31, float f32, float f33, float f34,
			float f41, float f42, float f43, float f44);

		Matrix(const Matrix& mat);

		// Access each element
		float& operator[] (const int iIndex);

		// Multiply this by another and return result
		const Matrix operator * (const Matrix& mat) const;

		// Multiply this by another
		const void operator *= (const Matrix& mat);

		const void operator = (const Matrix& mat);
		const bool operator == (const Matrix& mat) const;
		const bool operator != (const Matrix& mat) const;

		// Set to given values
		void set(float f11, float f12, float f13, float f14,
			float f21, float f22, float f23, float f24,
			float f31, float f32, float f33, float f34,
			float f41, float f42, float f43, float f44);

		// Zero all elements
		void zero(void);

		// Set matrix to identity
		void setIdentity(void);

		// Set rotation from a quaternion
		void setFromQuaternion(const Quaternion& quat);

		// Set rotation from axis angle
		void setFromAxisAngle(const Vector3& axis, float angle);

		// Set translation 
		void setTranslation(float x, float y, float z);

		// Set translation 
		void setTranslation(Vector3 translation);

		// Set the matrix up as an orthographic project for 2D rendering
		void setOrthographic(float fLeft = 0.0f, float fRight = (float)RenderDevice::getPointer()->getWindowWidth(), float fTop = 0.0f, float fBottom = (float)RenderDevice::getPointer()->getWindowHeight(), float fZnear = -1.0f, float fZfar = 1.0f);

		// Set the matrix up as a project matrix for OpenGL
		void setProjection(float fFOVdegrees, float fWidth, float fHeight, float fNear, float fFar);

		// Set the matrix up as a view matrix for OpenGL
		void setView(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& upVector);

		// Get translation
		void getTranslation(Vector3 &translation) const;

		// Fills this matrix with the current OpenGL model view matrix
		void getGLModelview(void);

		// Multiplies the current OpenGL model view matrix with this matrix
		void multiplyGLModelview(void);

		// Swap rows and columns
		void transpose(void);

		// Computes inverse
		void invert(void);

		float getDeterminant(void);

		// Translate the given vector using the matrix inverse
		void translateVecInv(Vector3& vector) const;

		// Rotate the given vector using the matrix inverse
		void rotateVecInv(Vector3& vector) const;

		// Get right vector
		void getRightVector(Vector3& vec);

		// Get up vector
		void getUpVector(Vector3& vec);

		// Get forward vector
		void getForwardVector(Vector3& vec);

		// Given a glm::mat4 matrix, sets this matrix as that.
		void convertFromGLM(const glm::mat4 &glm);

		float matrix[16];
	};

}
