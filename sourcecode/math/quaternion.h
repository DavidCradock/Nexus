#pragma once
#include "../precompiled_header.h"


namespace Nexus
{

	class Vector3;

	// Quaternion class for 3D rotations
	class Quaternion
	{
		friend class Matrix;
	public:
		// Default constructor, leaves quaternion in undefined state
		Quaternion();
		Quaternion(float x, float y, float z, float w);
		Quaternion(const Quaternion& quat);

		// Access each element
		float& operator[] (const int iIndex);

		// Multiply this by another and return result
		const Quaternion operator * (const Quaternion& quat) const;

		// Multiply this by another
		const void operator *= (const Quaternion& quat);

		// Add two quaternions, return result
		const Quaternion operator+(const Quaternion& quat) const;

		// Subtract two quaternions, return result
		const Quaternion operator-(const Quaternion& quat) const;

		// Multiply by scalar, return result
		const Quaternion operator*(const float fScalar) const;

		// Divide by scalar, return result
		const Quaternion operator/(const float fScalar) const;

		// Multiply a vector, return result
		Vector3 operator *(const Vector3& vec) const;

		const void operator = (const Quaternion& quat);
		const bool operator == (const Quaternion& quat) const;
		const bool operator != (const Quaternion& quat) const;

		float getMagnitude(void);
		void normalise(void);

		// Return this quaternion with it's xyz elements inverted.
		Quaternion getConjugate(void) const;

		// Returns this quaternion with all it's elements inverted.
		Quaternion getNegative(void) const;

		void setIdentity(void);

		void setFromAxisAngle(Vector3& vAxis, float fAngleRadians);
		Vector3 getAxis(void);
		float getAngle(void);

		void setFromEuler(float fAngleRadiansX, float fAngleRadiansY, float fAngleRadiansZ);

		// Returns a vector containing the amount of rotation around each local axis
		Vector3 getEuler(void);

		void setFromMatrix(const Matrix& mat);

		// Rotate this quaternion by another
		void rotate(const Quaternion& quat);

		// Return a quaternion which is interpolated between this quat and the one given using smooth SLERP
		Quaternion getSLERP(const Quaternion& quat, float fInterval) const;

		// Return a quaternion which is interpolated between this quat and the one given using linear interpolation
		Quaternion getLERP(const Quaternion& quat, float fInterval) const;

		float mfQuat[4];	// x,y,z,w
	};

}
