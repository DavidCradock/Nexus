#pragma once
#include "../precompiled_header.h"

namespace Nexus
{
	// A class to hold colour information. 8 bits for each colour component. Four colour components RGB and alpha
	class Colouruc
	{
	public:
		unsigned char r, g, b, a;

		// Default constructor (Sets to black colour with full alpha)
		inline Colouruc();

		// Constructor given four values
		inline Colouruc(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha = 255);

		// Copy constructor
		inline Colouruc(const Colouruc& colour);

		// Assignment
		inline Colouruc& operator =(const Colouruc& colour);

		// Set colour to given values
		inline void set(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha = 255);

		// Set colour to given values
		// The parsed float params should be in the range of 0.0f to 1.0f
		inline void setf(float fRed, float fGreen, float fBlue, float fAlpha = 1.0f);

		// Check for equality
		inline bool operator ==(const Colouruc& colour) const;

		// Check for inequality
		inline bool operator !=(const Colouruc& colour) const;
	};

	// A class to hold colour information. float for each colour component. Four colour components RGB and alpha
	class Colourf
	{
	public:
		float r, g, b, a;

		// Default constructor (Sets to black colour with full alpha)
		inline Colourf();

		// Constructor given four values
		inline Colourf(float fRed, float fGreen, float fBlue, float fAlpha = 1.0f);

		// Copy constructor
		inline Colourf(const Colourf& colour);

		// Assignment
		inline Colourf& operator =(const Colourf& colour);

		// Set colour to given values
		inline void set(float fRed, float fGreen, float fBlue, float fAlpha = 1.0f);

		// Set colour to given values
		// The parsed float params should be in the range of 0 to 255
		inline void setuc(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha = 255);

		// Check for equality
		inline bool operator ==(const Colourf& colour) const;

		// Check for inequality
		inline bool operator !=(const Colourf& colour) const;
	};

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	// Definition
	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	inline Colouruc::Colouruc()
	{
		r = g = b = 0;
		a = 255;
	}

	inline Colouruc::Colouruc(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha)
	{
		r = ucRed;
		g = ucGreen;
		b = ucBlue;
		a = ucAlpha;
	}

	// Copy constructor
	inline Colouruc::Colouruc(const Colouruc& colour)
	{
		r = colour.r;
		g = colour.g;
		b = colour.b;
		a = colour.a;
	}

	// Assignment
	inline Colouruc& Colouruc::operator =(const Colouruc& colour)
	{
		r = colour.r;
		g = colour.g;
		b = colour.b;
		a = colour.a;
		return *this;
	}

	inline void Colouruc::set(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha)
	{
		r = ucRed;
		g = ucGreen;
		b = ucBlue;
		a = ucAlpha;
	}

	inline void Colouruc::setf(float fRed, float fGreen, float fBlue, float fAlpha)
	{
		r = unsigned char(fRed * 255.0f);
		g = unsigned char(fGreen * 255.0f);
		b = unsigned char(fBlue * 255.0f);
		a = unsigned char(fAlpha * 255.0f);
	}

	inline bool Colouruc::operator ==(const Colouruc& colour) const
	{
		return r == colour.r && g == colour.g && b == colour.b && a == colour.a;
	}

	inline bool Colouruc::operator !=(const Colouruc& colour) const
	{
		return r != colour.r || g != colour.g || b != colour.b || a != colour.a;
	}

	inline Colourf::Colourf()
	{
		r = g = b = 0;
		a = 1.0f;
	}

	inline Colourf::Colourf(float fRed, float fGreen, float fBlue, float fAlpha)
	{
		r = fRed;
		g = fGreen;
		b = fBlue;
		a = fAlpha;
	}

	// Copy constructor
	inline Colourf::Colourf(const Colourf& colour)
	{
		r = colour.r;
		g = colour.g;
		b = colour.b;
		a = colour.a;
	}

	// Assignment
	inline Colourf& Colourf::operator =(const Colourf& colour)
	{
		r = colour.r;
		g = colour.g;
		b = colour.b;
		a = colour.a;
		return *this;
	}

	inline void Colourf::set(float fRed, float fGreen, float fBlue, float fAlpha)
	{
		r = fRed;
		g = fGreen;
		b = fBlue;
		a = fAlpha;
	}

	inline void Colourf::setuc(unsigned char ucRed, unsigned char ucGreen, unsigned char ucBlue, unsigned char ucAlpha)
	{
		float fMultiply = 1.0f / 255.0f;
		r = fMultiply * ucRed;
		g = fMultiply * ucGreen;
		b = fMultiply * ucBlue;
		a = fMultiply * ucAlpha;
	}

	inline bool Colourf::operator ==(const Colourf& colour) const
	{
		return r == colour.r && g == colour.g && b == colour.b && a == colour.a;
	}

	inline bool Colourf::operator !=(const Colourf& colour) const
	{
		return r != colour.r || g != colour.g || b != colour.b || a != colour.a;
	}
}
