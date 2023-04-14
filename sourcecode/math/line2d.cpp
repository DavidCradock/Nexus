#include "precompiled_header.h"
#include "line2d.h"

namespace Nexus
{

	CLine2DIntersectionResultLineLine::CLine2DIntersectionResultLineLine()
	{
		mbIntersectionOccurred = false;
	}

	CLine2DIntersectionResultLineCircle::CLine2DIntersectionResultLineCircle()
	{

	}

	CLine2D::CLine2D()
	{
	}

	CLine2D::CLine2D(const Vector2& vBeginPos, const Vector2& vEndPos)
	{
		begin = vBeginPos;
		end = vEndPos;
	}

	bool CLine2D::isPointLeft(Vector2 vPoint)
	{
		return ((end.x - begin.x) * (vPoint.y - begin.y) - (end.y - begin.y) * (vPoint.x - begin.x)) > 0;
	}

	bool CLine2D::intersectionTest(const CLine2D& otherLine)
	{
		float denom = ((otherLine.end.y - otherLine.begin.y) * (end.x - begin.x)) -
			((otherLine.end.x - otherLine.begin.x) * (end.y - begin.y));

		float nume_a = ((otherLine.end.x - otherLine.begin.x) * (begin.y - otherLine.begin.y)) -
			((otherLine.end.y - otherLine.begin.y) * (begin.x - otherLine.begin.x));

		float nume_b = ((end.x - begin.x) * (begin.y - otherLine.begin.y)) -
			((end.y - begin.y) * (begin.x - otherLine.begin.x));

		if (denom == 0.0f)
		{
			if (nume_a == 0.0f && nume_b == 0.0f)
			{
				// Coincident, IE are the same (maybe with endpoints reversed
				resultLineLine.mbIntersectionOccurred = false;
				return false;
			}
			// Parallel
			resultLineLine.mbIntersectionOccurred = false;
			return false;
		}

		float ua = nume_a / denom;
		float ub = nume_b / denom;

		if (ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f)
		{
			// We're intersecting
			// Get the intersection point.
			resultLineLine.mvIntersectionPoint.x = begin.x + ua * (end.x - begin.x);
			resultLineLine.mvIntersectionPoint.y = begin.y + ua * (end.y - begin.y);
			resultLineLine.mbIntersectionOccurred = true;
			return true;
		}

		// Not intersecting
		resultLineLine.mbIntersectionOccurred = true;
		return false;
	}

	bool CLine2D::intersectsCircle(const Vector2& vCirclePosition, float fCircleRadius)
	{
		float fA = (end.x - begin.x) * (end.x - begin.x) + (end.y - begin.y) * (end.y - begin.y);
		float fB = 2.0f * ((end.x - begin.x) * (begin.x - vCirclePosition.x) + (end.y - begin.y) * (begin.y - vCirclePosition.y));
		float fC = vCirclePosition.x * vCirclePosition.x + vCirclePosition.y * vCirclePosition.y + begin.x * begin.x + begin.y * begin.y - 2 * (vCirclePosition.x * begin.x + vCirclePosition.y * begin.y) - fCircleRadius * fCircleRadius;
		float fDeter = fB * fB - 4.0f * fA * fC;

		resultLineCircle.mbIntersects = false;		// Intersection occurred?
		resultLineCircle.mbInside = false;			// Both points inside circle?
		resultLineCircle.mbTangent = false;			// Only one intersection occurred?
		resultLineCircle.mvEnter.setZero();			// First intersection point
		resultLineCircle.mvExit.setZero();			// Second intersection point
		resultLineCircle.mbExitComputed = false;
		resultLineCircle.mbEnterComputed = false;

		if (fDeter <= 0)
		{
			resultLineCircle.mbInside = false;
		}
		else
		{
			float fE = sqrtf(fDeter);
			float fU1 = (-fB + fE) / (2.0f * fA);
			float fU2 = (-fB - fE) / (2.0f * fA);
			if ((fU1 < 0 || fU1 > 1) && (fU2 < 0 || fU2 > 1))
			{
				if ((fU1 < 0 && fU2 < 0) || (fU1 > 1 && fU2 > 1))
					resultLineCircle.mbInside = false;
				else
					resultLineCircle.mbInside = true;
			}
			else
			{
				if (0 <= fU2 && fU2 <= 1)
				{
					resultLineCircle.mvEnter = interpolate(begin, end, 1.0f - fU2);
					resultLineCircle.mbEnterComputed = true;
				}
				if (0 <= fU1 && fU1 <= 1)
				{
					resultLineCircle.mvExit = interpolate(begin, end, 1.0f - fU1);
					resultLineCircle.mbExitComputed = true;
				}
				resultLineCircle.mbIntersects = true;
				resultLineCircle.mbTangent = true;
				if (resultLineCircle.mbExitComputed && resultLineCircle.mbEnterComputed)
				{
					resultLineCircle.mbTangent = false;
				}
			}
		}

		return resultLineCircle.mbIntersects;
	}

}