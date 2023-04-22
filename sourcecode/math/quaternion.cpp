#include "precompiled_header.h"
#include "quaternion.h"
#include "vector3.h"
#include "matrix.h"

namespace Nexus
{

	Quaternion::Quaternion()
	{
	}

	Quaternion::Quaternion(float x, float y, float z, float w)
	{
		fQuat[0] = x;
		fQuat[1] = y;
		fQuat[2] = z;
		fQuat[3] = w;
	}

	Quaternion::Quaternion(const Quaternion& quat)
	{
		*this = quat;
	}

	float& Quaternion::operator [](const int iIndex)
	{
		return fQuat[iIndex];
	}

	const Quaternion Quaternion::operator *(const Quaternion& quat) const
	{
		/* Does the same as below? But too many sqrtf
		Vector3 v1(fQuat[0], fQuat[1], fQuat[2]);
		Vector3 v2(quat.fQuat[0], quat.fQuat[1], quat.fQuat[2]);
		Vector3 vFinal = v1 * quat.fQuat[3] + v2 * fQuat[3] + v1.getCross(v2);
		float fScalar = fQuat[3] * quat.fQuat[3] - v1.getDot(v2);
		return Quaternion(vFinal.x, vFinal.y, vFinal.z, fScalar);
		*/
		return Quaternion(fQuat[3] * quat.fQuat[0] + fQuat[0] * quat.fQuat[3] +
			fQuat[1] * quat.fQuat[2] - fQuat[2] * quat.fQuat[1],

			fQuat[3] * quat.fQuat[1] + fQuat[1] * quat.fQuat[3] +
			fQuat[2] * quat.fQuat[0] - fQuat[0] * quat.fQuat[2],

			fQuat[3] * quat.fQuat[2] + fQuat[2] * quat.fQuat[3] +
			fQuat[0] * quat.fQuat[1] - fQuat[1] * quat.fQuat[0],

			fQuat[3] * quat.fQuat[3] - fQuat[0] * quat.fQuat[0] -
			fQuat[1] * quat.fQuat[1] - fQuat[2] * quat.fQuat[2]);
	}

	const void Quaternion::operator *=(const Quaternion& quat)
	{
		Quaternion tmp(fQuat[3] * quat.fQuat[0] + fQuat[0] * quat.fQuat[3] +
			fQuat[1] * quat.fQuat[2] - fQuat[2] * quat.fQuat[1],

			fQuat[3] * quat.fQuat[1] + fQuat[1] * quat.fQuat[3] +
			fQuat[2] * quat.fQuat[0] - fQuat[0] * quat.fQuat[2],

			fQuat[3] * quat.fQuat[2] + fQuat[2] * quat.fQuat[3] +
			fQuat[0] * quat.fQuat[1] - fQuat[1] * quat.fQuat[0],

			fQuat[3] * quat.fQuat[3] - fQuat[0] * quat.fQuat[0] -
			fQuat[1] * quat.fQuat[1] - fQuat[2] * quat.fQuat[2]);
		*this = tmp;
	}

	const Quaternion Quaternion::operator +(const Quaternion& quat) const
	{
		return Quaternion(fQuat[0] + quat.fQuat[0], fQuat[1] + quat.fQuat[1],
			fQuat[2] + quat.fQuat[2], fQuat[3] + quat.fQuat[3]);
	}

	const Quaternion Quaternion::operator -(const Quaternion& quat) const
	{
		return Quaternion(fQuat[0] - quat.fQuat[0], fQuat[1] - quat.fQuat[1],
			fQuat[2] - quat.fQuat[2], fQuat[3] - quat.fQuat[3]);
	}

	const Quaternion Quaternion::operator *(const float fScalar) const
	{
		return Quaternion(fQuat[0] * fScalar, fQuat[1] * fScalar,
			fQuat[2] * fScalar, fQuat[3] * fScalar);
	}

	const Quaternion Quaternion::operator /(const float fScalar) const
	{
		float fInvScl = 1 / fScalar;
		return Quaternion(fQuat[0] * fInvScl, fQuat[1] * fInvScl,
			fQuat[2] * fInvScl, fQuat[3] * fInvScl);
	}

	Vector3 Quaternion::operator *(const Vector3& vec) const
	{
		// nVidia SDK implementation (Taken from OGRE)
		Vector3 v2, v3;
		Vector3 vQ(fQuat[0], fQuat[1], fQuat[2]);
		v2 = vQ.getCross(vec);
		v3 = vQ.getCross(v2);
		v2 *= (2.0f * fQuat[3]);
		v3 *= 2.0f;
		return vec + v2 + v3;
	}

	const void Quaternion::operator =(const Quaternion& quat)
	{
		memcpy(fQuat, quat.fQuat, sizeof(float[4]));
	}

	const bool Quaternion::operator ==(const Quaternion& quat) const
	{
		return (fQuat[0] == quat.fQuat[0] && fQuat[1] == quat.fQuat[1] && fQuat[2] == quat.fQuat[2] && fQuat[3] == quat.fQuat[3]);
	}

	const bool Quaternion::operator != (const Quaternion& quat) const
	{
		return !(*this == quat);
	}

	float Quaternion::getMagnitude(void)
	{
		return sqrtf((fQuat[0] * fQuat[0]) +
			(fQuat[1] * fQuat[1]) +
			(fQuat[2] * fQuat[2]) +
			(fQuat[3] * fQuat[3]));
	}

	void Quaternion::normalise(void)
	{
		float fInvMag = 1.0f / getMagnitude();
		if (fInvMag != 1.0f)
		{
			fQuat[0] *= fInvMag;
			fQuat[1] *= fInvMag;
			fQuat[2] *= fInvMag;
			fQuat[3] *= fInvMag;
		}
	}

	Quaternion Quaternion::getConjugate(void) const
	{
		return Quaternion(-fQuat[0], -fQuat[1], -fQuat[2], fQuat[3]);
	}

	Quaternion Quaternion::getNegative(void) const
	{
		return Quaternion(-fQuat[0], -fQuat[1], -fQuat[2], -fQuat[3]);
	}

	void Quaternion::setIdentity(void)
	{
		fQuat[0] = fQuat[1] = fQuat[2] = 0;
		fQuat[3] = 1;
	}

	void Quaternion::setFromAxisAngle(Vector3& vAxis, float fAngleRadians)
	{
		vAxis.normalise();
		float sa = sinf(fAngleRadians * 0.5f);
		float ca = cosf(fAngleRadians * 0.5f);
		fQuat[0] = vAxis.x * sa;
		fQuat[1] = vAxis.y * sa;
		fQuat[2] = vAxis.z * sa;
		fQuat[3] = ca;
	}

	Vector3 Quaternion::getAxis(void)
	{
		float sa = 1 / sqrtf(1 - (fQuat[4] * fQuat[4]));
		return Vector3(fQuat[0] * sa, fQuat[1] * sa, fQuat[2] * sa);
	}

	float Quaternion::getAngle(void)
	{
		return (float)2 * acosf(fQuat[3]);
	}

	void Quaternion::setFromEuler(float fAngleRadiansX, float fAngleRadiansY, float fAngleRadiansZ)
	{
		double dSY = sin(fAngleRadiansZ * 0.5f);
		double dSP = sin(fAngleRadiansY * 0.5f);
		double dSR = sin(fAngleRadiansX * 0.5f);
		double dCY = cos(fAngleRadiansZ * 0.5f);
		double dCP = cos(fAngleRadiansY * 0.5f);
		double dCR = cos(fAngleRadiansX * 0.5f);

		fQuat[0] = float(dSR * dCP * dCY - dCR * dSP * dSY);
		fQuat[1] = float(dCR * dSP * dCY + dSR * dCP * dSY);
		fQuat[2] = float(dCR * dCP * dSY - dSR * dSP * dCY);
		fQuat[3] = float(dCR * dCP * dCY + dSR * dSP * dSY);
		normalise();
	}

	Vector3 Quaternion::getEuler(void)
	{
		float fSq[4];
		fSq[0] = fQuat[0] * fQuat[0];
		fSq[1] = fQuat[1] * fQuat[1];
		fSq[2] = fQuat[2] * fQuat[2];
		fSq[3] = fQuat[3] * fQuat[3];

		double m11 = fSq[3] + fSq[0] - fSq[1] - fSq[2];
		double m21 = 2 * (fQuat[0] * fQuat[1] + fQuat[2] * fQuat[3]);
		double m31 = 2 * (fQuat[2] * fQuat[0] - fQuat[1] * fQuat[3]);
		double m32 = 2 * (fQuat[2] * fQuat[1] + fQuat[0] * fQuat[3]);
		double m33 = fSq[3] - fSq[0] - fSq[1] + fSq[2];
		return Vector3(float(atan2(m32, m33)), asinf(float(-m31)), float(atan2(m21, m11)));
	}

	void Quaternion::setFromMatrix(const Matrix& mat)
	{
		float fTr = mat.matrix[0] + mat.matrix[5] + mat.matrix[9] + 1.0f;
		// If the trace of the matrix is greater than 0, perform an instant calculation
		if (fTr > 0)
		{
			float fS = 0.5f / sqrtf(fTr);

			fQuat[3] = 0.25f / fS;
			fQuat[0] = (mat.matrix[9] - mat.matrix[6]) * fS;
			fQuat[1] = (mat.matrix[2] - mat.matrix[8]) * fS;
			fQuat[2] = (mat.matrix[4] - mat.matrix[1]) * fS;
		}
		// if the first diagonal element is the largest
		else if (mat.matrix[0] > mat.matrix[5] && mat.matrix[0] > mat.matrix[10])
		{
			float fS = float(1 / (sqrt(mat.matrix[0] - mat.matrix[5] - mat.matrix[10] + 1.0f) * 2));
			fQuat[0] = 0.5f * fS;
			fQuat[1] = (mat.matrix[1] + mat.matrix[4]) * fS;
			fQuat[2] = (mat.matrix[2] + mat.matrix[8]) * fS;
			fQuat[3] = (mat.matrix[6] + mat.matrix[9]) * fS;
		}
		// if the second.....
		else if (mat.matrix[5] > mat.matrix[10])
		{
			float fS = float(1 / (sqrt(mat.matrix[5] - mat.matrix[0] - mat.matrix[10] + 1.0f) * 2));
			fQuat[0] = (mat.matrix[1] + mat.matrix[4]) * fS;
			fQuat[1] = 0.5f * fS;
			fQuat[2] = (mat.matrix[6] + mat.matrix[9]) * fS;
			fQuat[3] = (mat.matrix[2] + mat.matrix[8]) * fS;
		}
		// if the last element is the largest
		else
		{
			float fS = float(1 / (sqrt(mat.matrix[10] - mat.matrix[0] - mat.matrix[5] + 1.0f) * 2));
			fQuat[0] = (mat.matrix[2] + mat.matrix[8]) * fS;
			fQuat[1] = (mat.matrix[6] + mat.matrix[9]) * fS;
			fQuat[2] = 0.5f * fS;
			fQuat[3] = (mat.matrix[1] + mat.matrix[4]) * fS;
		}
	}

	void Quaternion::rotate(const Quaternion& quat)
	{
		Quaternion tmp(quat * (*this) * quat.getConjugate());
		*this = tmp;
	}

	Quaternion Quaternion::getSLERP(const Quaternion& quat, float fInterval) const
	{
		Quaternion q(quat);

		float fDot = fQuat[0] * q.fQuat[0] + fQuat[1] * q.fQuat[1] + fQuat[2] * q.fQuat[2] + fQuat[3] * q.fQuat[3];
		if (fDot < 0.0f)
		{
			q = q.getNegative();
			fDot = -fDot;
		}

		// Inaccurate, use lerp instead
		if (fDot < 1.00001f && fDot > 0.99999f)
		{
			return getLERP(q, fInterval);
		}

		// Calculate the angle between the quaternions 
		float fTheta = acosf(fDot);

		return ((*this * sinf(fTheta * (1 - fInterval)) + q * sinf(fTheta * fInterval)) / sinf(fTheta));
	}

	Quaternion Quaternion::getLERP(const Quaternion& quat, float fInterval) const
	{
		Quaternion ret(((quat - *this) * fInterval) + *this);
		ret.normalise();
		return ret;
	}

}
