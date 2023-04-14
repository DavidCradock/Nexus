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
		mfQuat[0] = x;
		mfQuat[1] = y;
		mfQuat[2] = z;
		mfQuat[3] = w;
	}

	Quaternion::Quaternion(const Quaternion& quat)
	{
		*this = quat;
	}

	float& Quaternion::operator [](const int iIndex)
	{
		return mfQuat[iIndex];
	}

	const Quaternion Quaternion::operator *(const Quaternion& quat) const
	{
		/* Does the same as below? But too many sqrtf
		Vector3 v1(mfQuat[0], mfQuat[1], mfQuat[2]);
		Vector3 v2(quat.mfQuat[0], quat.mfQuat[1], quat.mfQuat[2]);
		Vector3 vFinal = v1 * quat.mfQuat[3] + v2 * mfQuat[3] + v1.getCross(v2);
		float fScalar = mfQuat[3] * quat.mfQuat[3] - v1.getDot(v2);
		return Quaternion(vFinal.x, vFinal.y, vFinal.z, fScalar);
		*/
		return Quaternion(mfQuat[3] * quat.mfQuat[0] + mfQuat[0] * quat.mfQuat[3] +
			mfQuat[1] * quat.mfQuat[2] - mfQuat[2] * quat.mfQuat[1],

			mfQuat[3] * quat.mfQuat[1] + mfQuat[1] * quat.mfQuat[3] +
			mfQuat[2] * quat.mfQuat[0] - mfQuat[0] * quat.mfQuat[2],

			mfQuat[3] * quat.mfQuat[2] + mfQuat[2] * quat.mfQuat[3] +
			mfQuat[0] * quat.mfQuat[1] - mfQuat[1] * quat.mfQuat[0],

			mfQuat[3] * quat.mfQuat[3] - mfQuat[0] * quat.mfQuat[0] -
			mfQuat[1] * quat.mfQuat[1] - mfQuat[2] * quat.mfQuat[2]);
	}

	const void Quaternion::operator *=(const Quaternion& quat)
	{
		Quaternion tmp(mfQuat[3] * quat.mfQuat[0] + mfQuat[0] * quat.mfQuat[3] +
			mfQuat[1] * quat.mfQuat[2] - mfQuat[2] * quat.mfQuat[1],

			mfQuat[3] * quat.mfQuat[1] + mfQuat[1] * quat.mfQuat[3] +
			mfQuat[2] * quat.mfQuat[0] - mfQuat[0] * quat.mfQuat[2],

			mfQuat[3] * quat.mfQuat[2] + mfQuat[2] * quat.mfQuat[3] +
			mfQuat[0] * quat.mfQuat[1] - mfQuat[1] * quat.mfQuat[0],

			mfQuat[3] * quat.mfQuat[3] - mfQuat[0] * quat.mfQuat[0] -
			mfQuat[1] * quat.mfQuat[1] - mfQuat[2] * quat.mfQuat[2]);
		*this = tmp;
	}

	const Quaternion Quaternion::operator +(const Quaternion& quat) const
	{
		return Quaternion(mfQuat[0] + quat.mfQuat[0], mfQuat[1] + quat.mfQuat[1],
			mfQuat[2] + quat.mfQuat[2], mfQuat[3] + quat.mfQuat[3]);
	}

	const Quaternion Quaternion::operator -(const Quaternion& quat) const
	{
		return Quaternion(mfQuat[0] - quat.mfQuat[0], mfQuat[1] - quat.mfQuat[1],
			mfQuat[2] - quat.mfQuat[2], mfQuat[3] - quat.mfQuat[3]);
	}

	const Quaternion Quaternion::operator *(const float fScalar) const
	{
		return Quaternion(mfQuat[0] * fScalar, mfQuat[1] * fScalar,
			mfQuat[2] * fScalar, mfQuat[3] * fScalar);
	}

	const Quaternion Quaternion::operator /(const float fScalar) const
	{
		float fInvScl = 1 / fScalar;
		return Quaternion(mfQuat[0] * fInvScl, mfQuat[1] * fInvScl,
			mfQuat[2] * fInvScl, mfQuat[3] * fInvScl);
	}

	Vector3 Quaternion::operator *(const Vector3& vec) const
	{
		// nVidia SDK implementation (Taken from OGRE)
		Vector3 v2, v3;
		Vector3 vQ(mfQuat[0], mfQuat[1], mfQuat[2]);
		v2 = vQ.getCross(vec);
		v3 = vQ.getCross(v2);
		v2 *= (2.0f * mfQuat[3]);
		v3 *= 2.0f;
		return vec + v2 + v3;
	}

	const void Quaternion::operator =(const Quaternion& quat)
	{
		memcpy(mfQuat, quat.mfQuat, sizeof(float[4]));
	}

	const bool Quaternion::operator ==(const Quaternion& quat) const
	{
		return (mfQuat[0] == quat.mfQuat[0] && mfQuat[1] == quat.mfQuat[1] && mfQuat[2] == quat.mfQuat[2] && mfQuat[3] == quat.mfQuat[3]);
	}

	const bool Quaternion::operator != (const Quaternion& quat) const
	{
		return !(*this == quat);
	}

	float Quaternion::getMagnitude(void)
	{
		return sqrtf((mfQuat[0] * mfQuat[0]) +
			(mfQuat[1] * mfQuat[1]) +
			(mfQuat[2] * mfQuat[2]) +
			(mfQuat[3] * mfQuat[3]));
	}

	void Quaternion::normalise(void)
	{
		float fInvMag = 1.0f / getMagnitude();
		if (fInvMag != 1.0f)
		{
			mfQuat[0] *= fInvMag;
			mfQuat[1] *= fInvMag;
			mfQuat[2] *= fInvMag;
			mfQuat[3] *= fInvMag;
		}
	}

	Quaternion Quaternion::getConjugate(void) const
	{
		return Quaternion(-mfQuat[0], -mfQuat[1], -mfQuat[2], mfQuat[3]);
	}

	Quaternion Quaternion::getNegative(void) const
	{
		return Quaternion(-mfQuat[0], -mfQuat[1], -mfQuat[2], -mfQuat[3]);
	}

	void Quaternion::setIdentity(void)
	{
		mfQuat[0] = mfQuat[1] = mfQuat[2] = 0;
		mfQuat[3] = 1;
	}

	void Quaternion::setFromAxisAngle(Vector3& vAxis, float fAngleRadians)
	{
		vAxis.normalise();
		float sa = sinf(fAngleRadians * 0.5f);
		float ca = cosf(fAngleRadians * 0.5f);
		mfQuat[0] = vAxis.x * sa;
		mfQuat[1] = vAxis.y * sa;
		mfQuat[2] = vAxis.z * sa;
		mfQuat[3] = ca;
	}

	Vector3 Quaternion::getAxis(void)
	{
		float sa = 1 / sqrtf(1 - (mfQuat[4] * mfQuat[4]));
		return Vector3(mfQuat[0] * sa, mfQuat[1] * sa, mfQuat[2] * sa);
	}

	float Quaternion::getAngle(void)
	{
		return (float)2 * acosf(mfQuat[3]);
	}

	void Quaternion::setFromEuler(float fAngleRadiansX, float fAngleRadiansY, float fAngleRadiansZ)
	{
		double dSY = sin(fAngleRadiansZ * 0.5f);
		double dSP = sin(fAngleRadiansY * 0.5f);
		double dSR = sin(fAngleRadiansX * 0.5f);
		double dCY = cos(fAngleRadiansZ * 0.5f);
		double dCP = cos(fAngleRadiansY * 0.5f);
		double dCR = cos(fAngleRadiansX * 0.5f);

		mfQuat[0] = float(dSR * dCP * dCY - dCR * dSP * dSY);
		mfQuat[1] = float(dCR * dSP * dCY + dSR * dCP * dSY);
		mfQuat[2] = float(dCR * dCP * dSY - dSR * dSP * dCY);
		mfQuat[3] = float(dCR * dCP * dCY + dSR * dSP * dSY);
		normalise();
	}

	Vector3 Quaternion::getEuler(void)
	{
		float fSq[4];
		fSq[0] = mfQuat[0] * mfQuat[0];
		fSq[1] = mfQuat[1] * mfQuat[1];
		fSq[2] = mfQuat[2] * mfQuat[2];
		fSq[3] = mfQuat[3] * mfQuat[3];

		double m11 = fSq[3] + fSq[0] - fSq[1] - fSq[2];
		double m21 = 2 * (mfQuat[0] * mfQuat[1] + mfQuat[2] * mfQuat[3]);
		double m31 = 2 * (mfQuat[2] * mfQuat[0] - mfQuat[1] * mfQuat[3]);
		double m32 = 2 * (mfQuat[2] * mfQuat[1] + mfQuat[0] * mfQuat[3]);
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

			mfQuat[3] = 0.25f / fS;
			mfQuat[0] = (mat.matrix[9] - mat.matrix[6]) * fS;
			mfQuat[1] = (mat.matrix[2] - mat.matrix[8]) * fS;
			mfQuat[2] = (mat.matrix[4] - mat.matrix[1]) * fS;
		}
		// if the first diagonal element is the largest
		else if (mat.matrix[0] > mat.matrix[5] && mat.matrix[0] > mat.matrix[10])
		{
			float fS = float(1 / (sqrt(mat.matrix[0] - mat.matrix[5] - mat.matrix[10] + 1.0f) * 2));
			mfQuat[0] = 0.5f * fS;
			mfQuat[1] = (mat.matrix[1] + mat.matrix[4]) * fS;
			mfQuat[2] = (mat.matrix[2] + mat.matrix[8]) * fS;
			mfQuat[3] = (mat.matrix[6] + mat.matrix[9]) * fS;
		}
		// if the second.....
		else if (mat.matrix[5] > mat.matrix[10])
		{
			float fS = float(1 / (sqrt(mat.matrix[5] - mat.matrix[0] - mat.matrix[10] + 1.0f) * 2));
			mfQuat[0] = (mat.matrix[1] + mat.matrix[4]) * fS;
			mfQuat[1] = 0.5f * fS;
			mfQuat[2] = (mat.matrix[6] + mat.matrix[9]) * fS;
			mfQuat[3] = (mat.matrix[2] + mat.matrix[8]) * fS;
		}
		// if the last element is the largest
		else
		{
			float fS = float(1 / (sqrt(mat.matrix[10] - mat.matrix[0] - mat.matrix[5] + 1.0f) * 2));
			mfQuat[0] = (mat.matrix[2] + mat.matrix[8]) * fS;
			mfQuat[1] = (mat.matrix[6] + mat.matrix[9]) * fS;
			mfQuat[2] = 0.5f * fS;
			mfQuat[3] = (mat.matrix[1] + mat.matrix[4]) * fS;
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

		float fDot = mfQuat[0] * q.mfQuat[0] + mfQuat[1] * q.mfQuat[1] + mfQuat[2] * q.mfQuat[2] + mfQuat[3] * q.mfQuat[3];
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
