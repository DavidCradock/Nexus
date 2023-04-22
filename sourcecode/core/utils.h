#pragma once
#include "../precompiled_header.h"

namespace Nexus
{

	// Sets the given colour vector based on given hue value (between zero and one)
	extern void getHueColour(float fHueAmount, float& fRed, float& fGreen, float& fBlue);

	// Returns true if the given disk filename exists.
	extern bool fileExists(const std::string& strFilename);

	// Writes a type to an already open FILE
	template<class Type> void writeToFile(FILE* f, Type& out)
	{
		fwrite(&out, sizeof(Type), 1, f);
	}

	// Converts the contents of a file into an array, stored inside a header file, for inclusion of external files, inside the executable of a program
	bool convertFileToHeader(std::string strFilename, std::string strArrayName = std::string("data"), unsigned int iNumElementsPerRow = 20);

	const float kPi = 3.14159265f;			//!< Pi constant
	const float k2Pi = kPi * 2.0f;			//!< 2 * Pi (Radians in a circle)
	const float kPiOver2 = kPi / 2.0f;		//!< Pi divided by 2 (90 degrees in radians)
	const float k1OverPi = 1.0f / kPi;
	const float k1Over2Pi = 1.0f / k2Pi;
	const float kPiOver180 = kPi / 180.0f;
	const float k180OverPi = 180.0f / kPi;

	// Maximum values for basic data types
	const double  kMaxDouble = (std::numeric_limits<double>::max)();
	const double  kMinDouble = (std::numeric_limits<double>::min)();
	const float   kMaxFloat = (std::numeric_limits<float>::max)();
	const float   kMinFloat = (std::numeric_limits<float>::min)();
	const int     kMaxInt = (std::numeric_limits<int>::max)();

	// Comparing a variable when it becomes invalid always returns false.
	// Nice easy way to check if I've done something stupid with a zero and a division somewhere
	template <typename Type>
	inline bool isNAN(Type type)
	{
		return type != type;
	}

	// Converts degrees to radians
	inline float deg2rad(const float fAngleDegrees) { return fAngleDegrees * kPiOver180; }

	// Converts radians to degrees
	inline float rad2deg(const float fAngleRadians) { return fAngleRadians * k180OverPi; }

	// Clamps given value within the range of -1 to +1 then performs standard acos function
	inline float acosClamped(float f)
	{
		if (f < -1.0f)
			return kPi;
		if (f > 1.0f)
			return 0.0f;
		return acosf(f);
	}

	// Computes both sin and cos of a scalar (Sometime faster to calculate both at same time)
	inline void sincos(float& outSin, float& outCos, float fScalar)
	{
		outSin = sinf(fScalar);
		outCos = cosf(fScalar);
	}

	// Clamps a value within given range
	template <class type>
	inline void clamp(type& valueToClamp, type valueA, type valueB)
	{
		type min;
		type max;
		if (valueA < valueB)
		{
			min = valueA;
			max = valueB;
		}
		else
		{
			min = valueB;
			max = valueA;
		}
		if (valueToClamp < min)
			valueToClamp = min;
		else if (valueToClamp > max)
			valueToClamp = max;
	}

	// Sets given variable to absolute value
	template <class type>
	inline void absolute(type& value)
	{
		if (value < 0)
			value *= -1;
	}

	// Computes the difference
	template <class type>
	inline type difference(type valueA, type valueB)
	{
		type result = valueA - valueB;
		absolute(result);
		return result;
	}

	// Returns a random integer between x and y
	inline int randInt(int x, int y) { return rand() % (y - x + 1) + x; }

	// Returns a random float between zero and 1
	inline float randFloat() { return float(rand()) / float(RAND_MAX + 1.0f); }

	// Returns a random double between zero and 1
	inline double randDouble() { return double(rand()) / double(RAND_MAX + 1.0); }

	// Returns a random bool
	inline bool randBool()
	{
		if (randInt(0, 1)) return true;
		return false;
	}

	// Returns a random float in the range -1 < n < 1
	inline double randomClamped() { return randFloat() - randFloat(); }

	// Returns a random float within the specified range
	inline float randf(float fMin, float fMax)
	{
		float fZeroToOne = (float)rand() / float(RAND_MAX + 1.0);
		return fMin + (fMax - fMin) * fZeroToOne;
	}
}