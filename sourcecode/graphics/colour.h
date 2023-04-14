#pragma once
#include "../precompiled_header.h"

namespace Nexus
{

	// A class to hold colour information. 8 bits for each colour component. Four colour components RGB and alpha
	class CColouruc
	{
	public:
		unsigned char r, g, b, a;

		// Default constructor (Sets to black colour with full alpha)
		inline CColouruc();

		// Constructor given four values
		inline CColouruc(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha = 255);

		// Copy constructor
		inline CColouruc(const CColouruc& colour);

		// Assignment
		inline CColouruc& operator =(const CColouruc& colour);

		// Set colour to given values
		inline void set(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha = 255);

		// Set colour to given values
		// The parsed float params should be in the range of 0.0f to 1.0f
		inline void setf(float fRed, float fGreen, float fBlue, float fAlpha = 1.0f);

		// Check for equality
		inline bool operator ==(const CColouruc& colour) const;

		// Check for inequality
		inline bool operator !=(const CColouruc& colour) const;

	private:

	};

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	// Definition
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	inline CColouruc::CColouruc()
	{
		r = g = b = 0;
		a = 255;
	}

	inline CColouruc::CColouruc(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha)
	{
		r = ucRed;
		g = ucGreen;
		b = ucBlue;
		a = ucAlpha;
	}

	// Copy constructor
	inline CColouruc::CColouruc(const CColouruc& colour)
	{
		r = colour.r;
		g = colour.g;
		b = colour.b;
		a = colour.a;
	}

	// Assignment
	inline CColouruc& CColouruc::operator =(const CColouruc& colour)
	{
		r = colour.r;
		g = colour.g;
		b = colour.b;
		a = colour.a;
		return *this;
	}

	inline void CColouruc::set(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha)
	{
		r = ucRed;
		g = ucGreen;
		b = ucBlue;
		a = ucAlpha;
	}

	inline void CColouruc::setf(float fRed, float fGreen, float fBlue, float fAlpha)
	{
		r = unsigned char(fRed * 255.0f);
		g = unsigned char(fGreen * 255.0f);
		b = unsigned char(fBlue * 255.0f);
		a = unsigned char(fAlpha * 255.0f);
	}

	inline bool CColouruc::operator ==(const CColouruc& colour) const
	{
		return r == colour.r && g == colour.g && b == colour.b && a == colour.a;
	}

	inline bool CColouruc::operator !=(const CColouruc& colour) const
	{
		return r != colour.r || g != colour.g || b != colour.b || a != colour.a;
	}

}
