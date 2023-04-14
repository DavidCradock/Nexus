#pragma once
#include "../precompiled_header.h"

namespace Nexus
{

	// A two dimensional vector
	class Vector2
	{
	public:
		float x, y;

		// Default constructor (Sets to zero vector)
		inline Vector2();

		// Construct given two values
		inline Vector2(float fx, float fy);

		// Copy constructor
		inline Vector2(const Vector2& vec);

		// Assignment
		inline Vector2& operator =(const Vector2& vec);

		// Set vector to given values
		inline void set(float fX, float fY) { x = fX; y = fY; }

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		inline const Vector2 operator *(const float f) const;

		// Multiplication by scala
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		inline void multiply(float fScaler);

		// Division by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		inline const Vector2 operator /(const float f) const;

		// Division by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		inline void divide(float fScaler);

		// CVector addition
		// CVector addition is commutative. (v1 + v2 == v2 + v1) == always true. 
		inline const Vector2 operator +(const Vector2& vec) const;

		// CVector subtraction
		// CVector addition is not commutative. (v1 - v2 == v2 - v1) == only true when v1 is same as v2. 
		inline const Vector2 operator -(const Vector2& vec) const;

		// CVector addition (Store result in this vector)
		// CVector addition is commutative. (v1 + v2 == v2 + v1) == always true. 
		inline Vector2& operator +=(const Vector2& vec);

		// CVector subtraction (Store result in this vector)
		// CVector addition is not commutative. (v1 - v2 == v2 - v1) == only true when v1 is same as v2. 
		inline Vector2& operator -=(const Vector2& vec);

		// Check for equality
		inline bool operator ==(const Vector2& vec) const;

		// Check for inequality
		inline bool operator !=(const Vector2& vec) const;

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

		// Computes and returns the vector's magnitude, squared. No squareroot, but still good for comparisons
		// A vector's magnitude is also know as "the length" or norm of the vector. 
		inline float getMagnitudeSquared(void) const;

		// Normalises this vector so that it becomes a unit vector (Has a magnitude of 1)
		inline void normalise(void);

		// Computes the dot product (also known as inner product) between this vector and the one given.
		// Dot product is commutative (v1.getDot(v2) == v2.getDot(v1)) == always true.
		// If returned dot product is...
		// > 0		vectors are pointing roughly in same direction (upto 90 degrees apart)
		// = 0		vectors are perpendicular to one another (90 degrees)
		// < 0		vectors are pointing in opposite directions (+90 degrees to 180 degrees)	
		inline float getDot(const Vector2& otherVec) const;

		// Computes the angle (in radians) between two UNIT VECTORS (Length of 1)
		// No checking of vectors are unit length here. 
		inline double getAngle(const Vector2& otherVec) const;

		// Computes distance between this vector and the one given, treating each vector as a point in 2D space.
		inline float getDistance(const Vector2& otherVec) const;

		// Computes distance squared between this vector and the one given, treating each vector as a point in 2D space.
		// This is faster than the getDistance() method, as no expensive square root is used.
		// This is useful if you have many vectors/points and need to find the closest out of them all.
		// After finding the closest, then you can square root the return value from this method or use getDistance() to obtain the true distance. 
		inline float getDistanceSquared(const Vector2& otherVec) const;

		// Returns a vector which is at right angle / 90 degrees / perpendicular to this one
		inline Vector2 getPerpendicular(void);

		// Determines a point between two specified points.
		// The parameter f determines where the new interpolated point is located relative to the two end points specified by parameters pt1 and pt2.
		// The closer the value of the parameter f is to 1.0, the closer the interpolated point is to the first point (parameter pt1).
		// The closer the value of the parameter f is to 0, the closer the interpolated point is to the second point (parameter pt2). 
		// Returns the new interpolated point
		inline Vector2 interpolate(const Vector2& v1, const Vector2& v2, float f);

		// Limits the length of this vector so that is equal to or below the parsed value
		// If the length of this vector does not exceed the one given, it remains unchanged.
		inline void limitLength(float fMaxLength);

		// If this vector represents a position, rotate it clockwise by the given number of degrees
		inline void rotateDeg(float fDegrees);

		// If this vector represents a position, rotate it clockwise by the given number of radians
		inline void rotateRad(float fRadians);

	private:
	};


	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	// Definition
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	inline Vector2::Vector2()
	{
		setZero();
	}

	inline Vector2::Vector2(float fx, float fy)
	{
		x = fx;
		y = fy;
	}

	inline Vector2::Vector2(const Vector2& vec)
	{
		x = vec.x;
		y = vec.y;
	}

	inline Vector2& Vector2::operator =(const Vector2& vec)
	{
		x = vec.x;
		y = vec.y;
		return *this;
	}

	inline const Vector2 Vector2::operator*(const float f) const
	{
		return Vector2(x * f, y * f);
	}

	inline void Vector2::multiply(float fScaler)
	{
		x *= fScaler;
		y *= fScaler;
	}

	inline void Vector2::divide(float fScaler)
	{
		x /= fScaler;
		y /= fScaler;

	}

	inline const Vector2 Vector2::operator +(const Vector2& vec) const
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	inline const Vector2 Vector2::operator -(const Vector2& vec) const
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	inline Vector2& Vector2::operator +=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;
		return *this;
	}

	inline Vector2& Vector2::operator -=(const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	inline bool Vector2::operator ==(const Vector2& vec) const
	{
		return x == vec.x && y == vec.y;
	}

	inline bool Vector2::operator !=(const Vector2& vec) const
	{
		return x != vec.x || y != vec.y;
	}

	inline void Vector2::setZero(void)
	{
		x = y = 0.0f;
	}

	inline bool Vector2::isZero(void) const
	{
		return x == 0 && y == 0;
	}

	inline void Vector2::negate(void)
	{
		x = -x;
		y = -y;
	}

	inline float Vector2::getMagnitude(void) const
	{
		float fMag = x * x;
		fMag += y * y;
		fMag = sqrtf(fMag);
		return fMag;
	}

	inline float Vector2::getMagnitudeSquared(void) const
	{
		float fMag = x * x;
		fMag += y * y;
		return fMag;
	}

	inline void Vector2::normalise(void)
	{
		float fMag = getMagnitude();
		if (fMag == 0.0f)
			return;
		float fReciprocal = 1.0f / fMag;
		x *= fReciprocal;
		y *= fReciprocal;
	}

	inline float Vector2::getDot(const Vector2& otherVec) const
	{
		return x * otherVec.x + y * otherVec.y;
	}

	inline double Vector2::getAngle(const Vector2& otherVec) const
	{
		return acos(getDot(otherVec));
	}

	inline float Vector2::getDistance(const Vector2& otherVec) const
	{
		float fx = x - otherVec.x;
		float fy = y - otherVec.y;
		return sqrtf(fx * fx + fy * fy);
	}

	inline float Vector2::getDistanceSquared(const Vector2& otherVec) const
	{
		float fx = x - otherVec.x;
		float fy = y - otherVec.y;
		return fx * fx + fy * fy;
	}

	inline Vector2 Vector2::getPerpendicular(void)
	{
		//Vector2 vPerpendicular(y, -x);
		Vector2 vPerpendicular(-y, x);
		return vPerpendicular;
	}

	inline Vector2 interpolate(const Vector2& v1, const Vector2& v2, float f)
	{
		if (f < 0.0f)
			return v2;
		else if (f > 1.0f)
			return v1;

		Vector2 vA = v1;
		vA.multiply(f);
		Vector2 vB = v2;
		vB.multiply(float(1.0f - f));
		return vA + vB;
	}

	inline void Vector2::limitLength(float fMaxLength)
	{
		float fLen = getMagnitude();
		if (fLen <= fMaxLength)
			return;
		normalise();
		multiply(fMaxLength);
	}

	// If this vector represents a position, rotate it clockwise by the given number of degrees
	void Vector2::rotateDeg(float fDegrees)
	{
		float fRad = fDegrees * -0.01745329251994329576923690768489f;
		float fOldX = x;
		float fOldY = y;
		float fSin, fCos;
		fSin = sinf(fRad);
		fCos = cosf(fRad);
		x = fOldX * fCos - fOldY * fSin;
		y = fOldX * fSin + fOldY * fCos;
	}

	// If this vector represents a position, rotate it clockwise by the given number of radians
	void Vector2::rotateRad(float fRadians)
	{
		float fOldX = x;
		float fOldY = y;
		float fSin, fCos;
		fSin = sinf(fRadians);
		fCos = cosf(fRadians);
		x = fOldX * fCos - fOldY * fSin;
		y = fOldX * fSin + fOldY * fCos;
	}

}
