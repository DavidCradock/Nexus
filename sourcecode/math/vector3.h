#pragma once
#include "../precompiled_header.h"
#include "matrix.h"
#include "quaternion.h"



namespace Nexus
{

	class Quaternion;
	class Matrix;

	// A three dimensional vector
	class Vector3
	{
	public:
		float x, y, z;

		// Default constructor (Sets to zero vector)
		inline Vector3();

		// Construct given three values
		inline Vector3(float fx, float fy, float fz);

		// Copy constructor
		inline Vector3(const Vector3& vec);

		// Assignment
		inline Vector3& operator =(const Vector3& vec);

		// Set vector to given values
		inline void set(float fX, float fY, float fZ) { x = fX; y = fY; z = fZ; }

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		inline const Vector3 operator *(const float f) const;

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		inline void operator *=(const float f);

		// Division by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		inline const Vector3 operator /(const float f) const;

		// CVector addition
		// CVector addition is commutative. (v1 + v2 == v2 + v1) == always true. 
		inline const Vector3 operator +(const Vector3& vec) const;

		// CVector subtraction
		// CVector addition is not commutative. (v1 - v2 == v2 - v1) == only true when v1 is same as v2. 
		inline const Vector3 operator -(const Vector3& vec) const;

		// CVector addition (Store result in this vector)
		// CVector addition is commutative. (v1 + v2 == v2 + v1) == always true. 
		inline Vector3& operator +=(const Vector3& vec);

		// CVector subtraction (Store result in this vector)
		// CVector addition is not commutative. (v1 - v2 == v2 - v1) == only true when v1 is same as v2. 
		inline Vector3& operator -=(const Vector3& vec);

		// Check for equality
		inline bool operator ==(const Vector3& vec) const;

		// Check for inequality
		inline bool operator !=(const Vector3& vec) const;

		// Sets each element within the vector's set to zero.
		// A zero vector is a special case. It does not have a direction, and it's magnitude is zero. 
		inline void setZero(void);

		// Returns true if the vector is a zero vector.
		inline bool isZero(void) const;

		// Negates the vector.
		// A negated vector has the same magnitude as before, however direction is reversed. 
		inline void negate(void);

		// Computes and returns the vector's magnitude
		// A vector's magnitude is also know as "the length" or norm of the vector. 
		inline float getMagnitude(void) const;

		// Normalises this vector so that it becomes a unit vector (Has a magnitude of 1)
		inline void normalise(void);

		// Computes the dot product (also known as inner product) between this vector and the one given.
		// Dot product is commutative (v1.getDot(v2) == v2.getDot(v1)) == always true.
		// If returned dot product is...
		// > 0		vectors are pointing roughly in same direction (upto 90 degrees apart)
		// = 0		vectors are perpendicular to one another (90 degrees)
		// < 0		vectors are pointing in opposite directions (+90 degrees to 180 degrees)	
		inline float getDot(const Vector3& otherVec) const;

		// Computes the cross product between this vector and the one given.
		// The cross product is a vector which is perpendicular to the two vectors it was calculated from. 
		inline Vector3 getCross(const Vector3& otherVec) const;

		// Computes the angle (in radians) between two UNIT VECTORS (Length of 1)
		// No checking of vectors are unit length here. 
		inline double getAngle(const Vector3& otherVec) const;

		// Computes distance between this vector and the one given, treating each vector as a point in 3D space.
		inline float getDistance(const Vector3& otherVec) const;

		// Computes distance squared between this vector and the one given, treating each vector as a point in 3D space.
		// This is faster than the getDistance() method, as no expensive square root is used.
		// This is useful if you have many vectors/points and need to find the closest out of them all.
		// After finding the closest, then you can squareroot the return value from this method or use getDistance() to obtain the true distance. 
		inline float getDistanceSquared(const Vector3& otherVec) const;

		// Transforms this vector by the given matrix using only the 3x3 elements containing the rotation
		inline void transform3(const Matrix& matrix);

		// Transforms this vector by the given matrix using both rotation and translation parts of the matrix
		inline void transform4(const Matrix& matrix);

		// Transforms this vector by the given quaternion
		inline void transformQ(const Quaternion& quat);

		inline void multiply(float f);
	private:
	};


	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	// Definition
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	inline Vector3::Vector3()
	{
		setZero();
	}

	inline Vector3::Vector3(float fx, float fy, float fz)
	{
		x = fx;
		y = fy;
		z = fz;
	}

	inline Vector3::Vector3(const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	inline Vector3& Vector3::operator =(const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	inline const Vector3 Vector3::operator*(const float f) const
	{
		return Vector3(x * f, y * f, z * f);
	}

	inline void Vector3::operator*=(const float f)
	{
		x = x * f;
		y = y * f;
		z = z * f;
	}



	inline const Vector3 Vector3::operator +(const Vector3& vec) const
	{
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	inline const Vector3 Vector3::operator -(const Vector3& vec) const
	{
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	inline Vector3& Vector3::operator +=(const Vector3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	inline Vector3& Vector3::operator -=(const Vector3& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	inline bool Vector3::operator ==(const Vector3& vec) const
	{
		return x == vec.x && y == vec.y && z == vec.z;
	}

	inline bool Vector3::operator !=(const Vector3& vec) const
	{
		return x != vec.x || y != vec.y || z != vec.z;
	}

	inline void Vector3::setZero(void)
	{
		x = y = z = 0.0f;
	}

	inline bool Vector3::isZero(void) const
	{
		return x == 0 && y == 0 && z == 0;
	}

	inline void Vector3::negate(void)
	{
		x = -x;
		y = -y;
		z = -z;
	}

	inline float Vector3::getMagnitude(void) const
	{
		float fMag = x * x;
		fMag += y * y;
		fMag += z * z;
		fMag = sqrtf(fMag);
		return fMag;
	}

	inline void Vector3::normalise(void)
	{
		float fMag = getMagnitude();
		if (fMag == 0.0f)
			return;
		float fReciprocal = 1.0f / fMag;
		x *= fReciprocal;
		y *= fReciprocal;
		z *= fReciprocal;
	}

	inline float Vector3::getDot(const Vector3& otherVec) const
	{
		return x * otherVec.x + y * otherVec.y + z * otherVec.z;
	}

	inline double Vector3::getAngle(const Vector3& otherVec) const
	{
		return acos(getDot(otherVec));
	}

	inline Vector3 Vector3::getCross(const Vector3& otherVec) const
	{
		Vector3 vCross;
		vCross.x = y * otherVec.z - z * otherVec.y;
		vCross.y = z * otherVec.x - x * otherVec.z;
		vCross.z = x * otherVec.y - y * otherVec.x;
		return vCross;
	}

	inline float Vector3::getDistance(const Vector3& otherVec) const
	{
		float fx = x - otherVec.x;
		float fy = y - otherVec.y;
		float fz = z - otherVec.z;
		return sqrtf(fx * fx + fy * fy + fz * fz);
	}

	inline float Vector3::getDistanceSquared(const Vector3& otherVec) const
	{
		float fx = x - otherVec.x;
		float fy = y - otherVec.y;
		float fz = z - otherVec.z;
		return fx * fx + fy * fy + fz * fz;
	}

	inline void Vector3::transform3(const Matrix& matrix)
	{
		Vector3 temp((x * matrix.matrix[0] +
			y * matrix.matrix[4] +
			z * matrix.matrix[8]),

			(x * matrix.matrix[1] +
				y * matrix.matrix[5] +
				z * matrix.matrix[9]),

			(x * matrix.matrix[2] +
				y * matrix.matrix[6] +
				z * matrix.matrix[10]));
		*this = temp;
	}

	inline void Vector3::transform4(const Matrix& matrix)
	{
		Vector3 temp((x * matrix.matrix[0] +
			y * matrix.matrix[4] +
			z * matrix.matrix[8] +
			matrix.matrix[12]),

			(x * matrix.matrix[1] +
				y * matrix.matrix[5] +
				z * matrix.matrix[9] +
				matrix.matrix[13]),

			(x * matrix.matrix[2] +
				y * matrix.matrix[6] +
				z * matrix.matrix[10] +
				matrix.matrix[14]));
		*this = temp;
	}

	inline void Vector3::transformQ(const Quaternion& quat)
	{
		Quaternion qv(x, y, z, 0);
		Quaternion qF = quat * qv * quat.getConjugate();
		x = qF.fQuat[0];
		y = qF.fQuat[1];
		z = qF.fQuat[2];
	}

	inline void Vector3::multiply(float f)
	{
		x *= f;
		y *= f;
		z *= f;
	}

}
