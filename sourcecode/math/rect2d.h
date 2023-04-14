#pragma once
#include "../precompiled_header.h"
#include "vector2.h"

namespace Nexus
{

	// For checking 2D rectangular region collision/intersection
	class Rect2D
	{
	public:
		Rect2D()
		{
			left = right = top = bottom = 0;
		}

		Rect2D(const Vector2& vPosMin, const Vector2& vPosMax)
		{
			left = int(vPosMin.x);
			top = int(vPosMin.y);
			right = int(vPosMax.x);
			bottom = int(vPosMax.y);
		}

		Rect2D(int iLeft, int iRight, int iTop, int iBottom)
		{
			left = iLeft;
			right = iRight;
			top = iTop;
			bottom = iBottom;
		}

		// Returns true if this rect collides with the other rect
		inline bool collision(const Rect2D& otherRect)
		{
			if (right < otherRect.left)
				return false;
			if (left > otherRect.right)
				return false;
			if (bottom < otherRect.top)
				return false;
			if (top > otherRect.bottom)
				return false;
			return true;
		}

		int left, right, top, bottom;
	};

}