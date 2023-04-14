#pragma once
#include "../precompiled_header.h"
#include "matrix.h"
#include "quaternion.h"

namespace Nexus
{
	// A four dimensional vector
	class Vector4
	{
	public:
		float x, y, z, w;

		// Default constructor (Sets to zero vector)
		inline Vector4();

		// Construct given four values
		inline Vector4(float fx, float fy, float fz, float fw);

		// Copy constructor
		inline Vector4(const Vector4& vec);

		// Assignment
		inline Vector4& operator =(const Vector4& vec);

		// Set vector to given values
		inline void set(float fX, float fY, float fZ, float fw) { x = fX; y = fY; z = fZ; w = fw; }

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		inline const Vector4 operator *(const float f) const;

		// Multiplication by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		inline void operator *=(const float f);

		// Division by scalar
		// Adjusts the magnitude of a vector, keeping the direction the same (Unless the scalar is negative, in which case the direction is reversed) 
		inline const Vector4 operator /(const float f) const;

		// CVector addition
		// CVector addition is commutative. (v1 + v2 == v2 + v1) == always true. 
		inline const Vector4 operator +(const Vector4& vec) const;

		// CVector subtraction
		// CVector addition is not commutative. (v1 - v2 == v2 - v1) == only true when v1 is same as v2. 
		inline const Vector4 operator -(const Vector4& vec) const;

		// CVector addition (Store result in this vector)
		// CVector addition is commutative. (v1 + v2 == v2 + v1) == always true. 
		inline Vector4& operator +=(const Vector4& vec);

		// CVector subtraction (Store result in this vector)
		// CVector addition is not commutative. (v1 - v2 == v2 - v1) == only true when v1 is same as v2. 
		inline Vector4& operator -=(const Vector4& vec);

		// Check for equality
		inline bool operator ==(const Vector4& vec) const;

		// Check for inequality
		inline bool operator !=(const Vector4& vec) const;

		// Sets each element within the vector's set to zero.
		// A zero vector is a special case. It does not have a direction, and it's magnitude is zero. 
		inline void setZero(void);

		// Returns true if the vector is a zero vector.
		inline bool isZero(void) const;

		// Negates the vector.
		// A negated vector has the same magnitude as before, however direction is reversed. 
		inline void negate(void);

	private:
	};


	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	// Definition
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	inline Vector4::Vector4()
	{
		setZero();
	}

	inline Vector4::Vector4(float fx, float fy, float fz, float fw)
	{
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	}

	inline Vector4::Vector4(const Vector4& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
	}

	inline Vector4& Vector4::operator =(const Vector4& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
		return *this;
	}

	inline const Vector4 Vector4::operator*(const float f) const
	{
		return Vector4(x * f, y * f, z * f, w * f);
	}

	inline void Vector4::operator*=(const float f)
	{
		x = x * f;
		y = y * f;
		z = z * f;
		w = w * f;
	}



	inline const Vector4 Vector4::operator +(const Vector4& vec) const
	{
		return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
	}

	inline const Vector4 Vector4::operator -(const Vector4& vec) const
	{
		return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
	}

	inline Vector4& Vector4::operator +=(const Vector4& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.w;
		return *this;
	}

	inline Vector4& Vector4::operator -=(const Vector4& vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		w -= vec.w;
		return *this;
	}

	inline bool Vector4::operator ==(const Vector4& vec) const
	{
		return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
	}

	inline bool Vector4::operator !=(const Vector4& vec) const
	{
		return x != vec.x || y != vec.y || z != vec.z || w != vec.w;
	}

	inline void Vector4::setZero(void)
	{
		x = y = z = w = 0.0f;
	}

	inline bool Vector4::isZero(void) const
	{
		return x == 0 && y == 0 && z == 0 && w == 0;
	}

	inline void Vector4::negate(void)
	{
		x = -x;
		y = -y;
		z = -z;
		w = -w;
	}

}