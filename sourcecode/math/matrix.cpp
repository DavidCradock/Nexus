#include "precompiled_header.h"
#include "matrix.h"
#include "quaternion.h"
#include "vector3.h"

namespace Nexus
{
	Matrix::Matrix()
	{
		setIdentity();
	}

	Matrix::Matrix(float f11, float f12, float f13, float f14,
		float f21, float f22, float f23, float f24,
		float f31, float f32, float f33, float f34,
		float f41, float f42, float f43, float f44)
	{
		matrix[0] = f11;		matrix[1] = f12;		matrix[2] = f13;		matrix[3] = f14;
		matrix[4] = f21;		matrix[5] = f22;		matrix[6] = f23;		matrix[7] = f24;
		matrix[8] = f31;		matrix[9] = f32;		matrix[10] = f33;		matrix[11] = f34;
		matrix[12] = f41;		matrix[13] = f42;		matrix[14] = f43;		matrix[15] = f44;
	}

	Matrix::Matrix(const Matrix& mat)
	{
		*this = mat;
	}

	float& Matrix::operator [](const int iIndex)
	{
		return matrix[iIndex];
	}

	const Matrix Matrix::operator *(const Matrix& mat) const
	{
		//Shorten the syntax a bit
		const float* m1 = matrix;
		const float* m2 = mat.matrix;

		return Matrix(

			(m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3]),
			(m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3]),
			(m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3]),
			(m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3]),

			(m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7]),
			(m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7]),
			(m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7]),
			(m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7]),

			(m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11]),
			(m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11]),
			(m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11]),
			(m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11]),

			(m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15]),
			(m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15]),
			(m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15]),
			(m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15])
		);
	}

	const void Matrix::operator *=(const Matrix& mat)
	{
		float fNewMat[16];
		const float* m1 = matrix;
		const float* m2 = mat.matrix;

		fNewMat[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
		fNewMat[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
		fNewMat[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
		fNewMat[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

		fNewMat[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
		fNewMat[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
		fNewMat[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
		fNewMat[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

		fNewMat[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
		fNewMat[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
		fNewMat[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
		fNewMat[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

		fNewMat[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
		fNewMat[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
		fNewMat[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
		fNewMat[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

		memcpy(matrix, fNewMat, sizeof(float[16]));
	}

	const void Matrix::operator =(const Matrix& mat)
	{
		memcpy(matrix, mat.matrix, sizeof(float[16]));
	}

	const bool Matrix::operator ==(const Matrix& mat) const
	{
		return (matrix[0] == mat.matrix[0] && matrix[1] == mat.matrix[1] && matrix[2] == mat.matrix[2] && matrix[3] == mat.matrix[3] &&
			matrix[4] == mat.matrix[4] && matrix[5] == mat.matrix[5] && matrix[6] == mat.matrix[6] && matrix[7] == mat.matrix[7] &&
			matrix[8] == mat.matrix[8] && matrix[9] == mat.matrix[9] && matrix[10] == mat.matrix[10] && matrix[11] == mat.matrix[11] &&
			matrix[12] == mat.matrix[12] && matrix[13] == mat.matrix[13] && matrix[14] == mat.matrix[14] && matrix[15] == mat.matrix[15]);
	}

	const bool Matrix::operator != (const Matrix& mat) const
	{
		return !(*this == mat);
	}

	void Matrix::set(float f11, float f12, float f13, float f14,
		float f21, float f22, float f23, float f24,
		float f31, float f32, float f33, float f34,
		float f41, float f42, float f43, float f44)
	{
		matrix[0] = f11;		matrix[1] = f12;		matrix[2] = f13;		matrix[3] = f14;
		matrix[4] = f21;		matrix[5] = f22;		matrix[6] = f23;		matrix[7] = f24;
		matrix[8] = f31;		matrix[9] = f32;		matrix[10] = f33;	matrix[11] = f34;
		matrix[12] = f41;	matrix[13] = f42;	matrix[14] = f43;	matrix[15] = f44;
	}

	void Matrix::zero(void)
	{
		memset(matrix, 0, sizeof(float[16]));
	}

	void Matrix::setIdentity(void)
	{
		zero();
		matrix[0] = 1.0f;
		matrix[5] = 1.0f;
		matrix[10] = 1.0f;
		matrix[15] = 1.0f;
	}

	void Matrix::setFromQuaternion(const Quaternion& quat)
	{
		matrix[0] = 1.0f - 2.0f * (quat.fQuat[1] * quat.fQuat[1] + quat.fQuat[2] * quat.fQuat[2]);
		matrix[1] = 2.0f * (quat.fQuat[0] * quat.fQuat[1] - quat.fQuat[2] * quat.fQuat[3]);
		matrix[2] = 2.0f * (quat.fQuat[0] * quat.fQuat[2] + quat.fQuat[1] * quat.fQuat[3]);

		matrix[4] = 2.0f * (quat.fQuat[0] * quat.fQuat[1] + quat.fQuat[2] * quat.fQuat[3]);
		matrix[5] = 1.0f - 2.0f * (quat.fQuat[0] * quat.fQuat[0] + quat.fQuat[2] * quat.fQuat[2]);
		matrix[6] = 2.0f * (quat.fQuat[1] * quat.fQuat[2] - quat.fQuat[0] * quat.fQuat[3]);

		matrix[8] = 2.0f * (quat.fQuat[0] * quat.fQuat[2] - quat.fQuat[1] * quat.fQuat[3]);
		matrix[9] = 2.0f * (quat.fQuat[1] * quat.fQuat[2] + quat.fQuat[0] * quat.fQuat[3]);
		matrix[10] = 1.0f - 2.0f * (quat.fQuat[0] * quat.fQuat[0] + quat.fQuat[1] * quat.fQuat[1]);

		matrix[15] = 1.0f;
	}

	void Matrix::setFromAxisAngle(const Vector3& axis, float angle)
	{
		float fCos = cosf(angle);
		float fSin = sinf(angle);
		float fOMC = 1.0f - fCos;

		matrix[0] = fCos + (axis.x * axis.x) * fOMC;
		matrix[5] = fCos + (axis.y * axis.y) * fOMC;
		matrix[10] = fCos + (axis.z * axis.z) * fOMC;
		matrix[15] = 1.0f;
		matrix[4] = axis.x * axis.y * fOMC + axis.z * fSin;
		matrix[1] = axis.x * axis.y * fOMC - axis.z * fSin;
		matrix[8] = axis.x * axis.z * fOMC + axis.y * fSin;
		matrix[2] = axis.x * axis.z * fOMC - axis.y * fSin;
		matrix[9] = axis.y * axis.z * fOMC + axis.x * fSin;
		matrix[6] = axis.y * axis.z * fOMC - axis.x * fSin;
	}

	void Matrix::setTranslation(float x, float y, float z)
	{
		matrix[12] = x;
		matrix[13] = y;
		matrix[14] = z;
	}

	void Matrix::getTranslation(Vector3 &translation) const
	{
		translation.x = matrix[12];
		translation.y = matrix[13];
		translation.z = matrix[14];
	}

	void Matrix::setTranslation(Vector3 translation)
	{
		matrix[12] = translation.x;
		matrix[13] = translation.y;
		matrix[14] = translation.z;
	}

	void Matrix::setOrthographic(float fLeft, float fRight, float fTop, float fBottom, float fZnear, float fZfar)
	{
		setIdentity();
		/* From GLM
		Result[0][0] = static_cast<T>(2) / (right - left);
		Result[1][1] = static_cast<T>(2) / (top - bottom);
		Result[2][2] = -static_cast<T>(2) / (zFar - zNear);
		Result[3][0] = -(right + left) / (right - left);
		Result[3][1] = -(top + bottom) / (top - bottom);
		Result[3][2] = -(zFar + zNear) / (zFar - zNear);
		*/
		matrix[0] = 2.0f / (fRight - fLeft);
		matrix[5] = 2.0f / (fTop - fBottom);
		matrix[10] = -2.0f / (fZfar - fZnear);
		matrix[12] = -(fRight - fLeft) / (fRight - fLeft);
		matrix[13] = -(fTop + fBottom) / (fTop - fBottom);
		matrix[14] = -(fZfar + fZnear) / (fZfar - fZnear);
	}

	void Matrix::setProjection(float fFOVdegrees, float fWidth, float fHeight, float fNear, float fFar)
	{
		glm::mat4 matProjectionGLM = glm::perspective(glm::radians(fFOVdegrees), float(fWidth / (float)fHeight), fNear, fFar);
		convertFromGLM(matProjectionGLM);
		/*
		// Compute aspect ratio
		float fAspect = fWidth / fHeight;

		// Taken from asking bard to "how to create a projection matrix for OpenGL please?"
		// and then "Without using GLM"
		matrix[0] = 2.0f * fNear / (fAspect * tan(fFOVdegrees * 0.5f));
		matrix[1] = 0.0f;
		matrix[2] = 0.0f;
		matrix[3] = 0.0f;

		matrix[4] = 0.0f;
		matrix[5] = 2.0f * fNear / tan(fFOVdegrees * 0.5f);
		matrix[6] = 0.0f;
		matrix[7] = 0.0f;

		matrix[8] = 0.0f;
		matrix[9] = 0.0f;
		matrix[10] = -(fFar + fNear) / (fFar - fNear);
		matrix[11] = -1.0f;

		matrix[12] = 0.0f;
		matrix[13] = 0.0f;
		matrix[14] = -2.0f * fFar * fNear / (fFar - fNear);
		matrix[15] = 0.0f;
		*/
	}

	void Matrix::setView(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& upVector)
	{
		glm::mat4 matViewGLM;
		matViewGLM = glm::lookAt(
			glm::vec3(eyePosition.x, eyePosition.y, eyePosition.z),
			glm::vec3(targetPosition.x, targetPosition.y, targetPosition.z),
			glm::vec3(upVector.x, upVector.y, upVector.z));
		convertFromGLM(matViewGLM);
/*
		// Calculate the view matrix.
		matrix[0] = eyePosition.x;
		matrix[1] = eyePosition.y;
		matrix[2] = eyePosition.z;
		matrix[3] = 0.0f;

		matrix[4] = upVector.x;
		matrix[5] = upVector.x;
		matrix[6] = upVector.x;
		matrix[7] = 0.0f;

		matrix[8] = -(targetPosition.x - eyePosition.x);
		matrix[9] = -(targetPosition.y - eyePosition.y);
		matrix[10] = -(targetPosition.z - eyePosition.z);
		matrix[11] = 1.0f;

		matrix[12] = 0.0f;
		matrix[13] = 0.0f;
		matrix[14] = 0.0f;
		matrix[15] = 1.0f;
		*/
	}

	void Matrix::convertFromGLM(const glm::mat4 &glm)
	{
		matrix[0] = glm[0][0];
		matrix[1] = glm[0][1];
		matrix[2] = glm[0][2];
		matrix[3] = glm[0][3];
		matrix[4] = glm[1][0];
		matrix[5] = glm[1][1];
		matrix[6] = glm[1][2];
		matrix[7] = glm[1][3];
		matrix[8] = glm[2][0];
		matrix[9] = glm[2][1];
		matrix[10] = glm[2][2];
		matrix[11] = glm[2][3];
		matrix[12] = glm[3][0];
		matrix[13] = glm[3][1];
		matrix[14] = glm[3][2];
		matrix[15] = glm[3][3];

	}

	void Matrix::getGLModelview(void)
	{
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	}

	void Matrix::multiplyGLModelview(void)
	{
		glMultMatrixf(matrix);
	}

	void Matrix::transpose(void)
	{
		Matrix tmp(matrix[0], matrix[4], matrix[8], matrix[12],
			matrix[1], matrix[5], matrix[9], matrix[13],
			matrix[2], matrix[6], matrix[10], matrix[14],
			matrix[3], matrix[7], matrix[11], matrix[15]);
		*this = tmp;
	}

	//- ADAPTED FROM MTXLIB.CPP on the Game Programming Gems CD
	//- Original code Copyright (C) Dante Treglia II and Mark A. DeLoura, 2000
	void Matrix::invert(void)
	{
		Matrix ma(*this);
		Matrix mb;
		mb.setIdentity();

		unsigned int uiRow, uiColumn;
		unsigned int uiRowMax; // Points to max abs value row in this column
		float tmp;

		// Go through columns
		for (uiColumn = 0; uiColumn < 4; ++uiColumn)
		{
			// Find the row with max value in this column
			uiRowMax = uiColumn;
			for (uiRow = uiColumn + 1; uiRow < 4; ++uiRow)
			{
				if (fabs(ma[4 * uiRow + uiColumn]) > fabs(ma[4 * uiRowMax + uiColumn]))
				{
					uiRowMax = uiRow;
				}
			}

			// If the max value here is 0, we can't invert.  .
			if (ma[4 * uiColumn + uiRowMax] == 0.0F)
			{
				setIdentity();
				return;
			}

			// Swap row "rowMax" with row "c"
			for (int cc = 0; cc < 4; ++cc)
			{
				tmp = ma[4 * uiColumn + cc];
				ma[4 * uiColumn + cc] = ma[4 * uiRowMax + cc];
				ma[4 * uiRowMax + cc] = tmp;

				tmp = mb[4 * uiColumn + cc];
				mb[4 * uiColumn + cc] = mb[4 * uiRowMax + cc];
				mb[4 * uiRowMax + cc] = tmp;
			}

			// Now everything we do is on row "c".
			// Set the max cell to 1 by dividing the entire row by that value
			tmp = ma[4 * uiColumn + uiColumn];
			for (int cc = 0; cc < 4; ++cc)
			{
				ma[4 * uiColumn + cc] /= tmp;
				mb[4 * uiColumn + cc] /= tmp;
			}

			// Now do the other rows, so that this column only has a 1 and 0's
			for (uiRow = 0; uiRow < 4; ++uiRow)
			{
				if (uiRow != uiColumn)
				{
					tmp = ma[4 * uiRow + uiColumn];
					for (int cc = 0; cc < 4; ++cc)
					{
						ma[4 * uiRow + cc] -= ma[4 * uiColumn + cc] * tmp;
						mb[4 * uiRow + cc] -= mb[4 * uiColumn + cc] * tmp;
					}
				}
			}
		}
		*this = mb;
	}

	float Matrix::getDeterminant(void)
	{
		return((matrix[0] * matrix[5] * matrix[10] * matrix[15]) +
			(matrix[1] * matrix[6] * matrix[11] * matrix[12]) +
			(matrix[2] * matrix[7] * matrix[8] * matrix[13]) +
			(matrix[3] * matrix[4] * matrix[9] * matrix[14]) -
			(matrix[3] * matrix[6] * matrix[9] * matrix[12]) -
			(matrix[7] * matrix[10] * matrix[13] * matrix[0]) -
			(matrix[11] * matrix[14] * matrix[1] * matrix[4]) -
			(matrix[15] * matrix[2] * matrix[5] * matrix[8]));
	}

	void Matrix::translateVecInv(Vector3& vector) const
	{
		vector.x = vector.x - matrix[12];
		vector.y = vector.y - matrix[13];
		vector.z = vector.z - matrix[14];
	}

	void Matrix::rotateVecInv(Vector3& vector) const
	{
		Vector3 vTmp(vector);
		vector.x = vTmp.x * matrix[0] + vTmp.y * matrix[1] + vTmp.z * matrix[2];
		vector.y = vTmp.x * matrix[4] + vTmp.y * matrix[5] + vTmp.z * matrix[6];
		vector.z = vTmp.x * matrix[8] + vTmp.y * matrix[9] + vTmp.z * matrix[10];
	}

	void Matrix::getRightVector(Vector3& vec)
	{
		vec.x = matrix[0];
		vec.y = matrix[1];
		vec.z = matrix[2];
	}

	void Matrix::getUpVector(Vector3& vec)
	{
		vec.x = matrix[4];
		vec.y = matrix[5];
		vec.z = matrix[6];
	}

	void Matrix::getForwardVector(Vector3& vec)
	{
		vec.x = matrix[8];
		vec.y = matrix[9];
		vec.z = matrix[10];
	}

}
