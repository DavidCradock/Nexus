#pragma once
#include "../precompiled_header.h"
#include "vector2.h"

namespace Nexus
{

	// Used by CLine2D to hold intersection results for line-line
	class CLine2DIntersectionResultLineLine
	{
	public:
		CLine2DIntersectionResultLineLine();

		bool mbIntersectionOccurred;	// Whether there was an intersection or not
		Vector2 mvIntersectionPoint;	// If bIntersection is true, this holds the point of intersection
	};

	// Used by CLine2D to hold intersection results for line-circle
	class CLine2DIntersectionResultLineCircle
	{
	public:
		CLine2DIntersectionResultLineCircle();

		bool mbInside;			// Is true if both line points are inside the circle
		bool mbIntersects;		// Is true if at least one intersection point was found
		bool mbTangent;			// Is true if only one intersection point was found (Use mbEnterComputed and mbExitComputed to determine which)
		bool mbEnterComputed;	// mvEnter intersection point has been found
		bool mbExitComputed;	// mvExit intersection point has been found
		Vector2 mvEnter;		// Holds "enter" intersection point (if mvEnterComputed == true)
		Vector2 mvExit;		// Holds "exit" intersection point (if mvExitComputed == true)

	};

	// A class which represents a 2D line
	// Usefull for 2D line intersection tests and circle line intersection tests
	class CLine2D
	{
	public:
		CLine2D();

		// Construct the line
		CLine2D(const Vector2& vBeginPos, const Vector2& vEndPos);

		Vector2 begin;					// Beginning point
		Vector2 end;					// End point

		CLine2DIntersectionResultLineLine resultLineLine;		// Will hold intersection test result information for line/line intersection test
		CLine2DIntersectionResultLineCircle resultLineCircle;	// Will hold intersection test result information for line/circle intersection test

		// Returns true if the given point is "left" of this line, else false
		bool isPointLeft(Vector2 vPoint);

		// Returns true if the parsed line intersects this one
		// If true is returned, intersectionResult member holds information about the intersection
		bool intersectionTest(const CLine2D& otherLine);

		// Returns true if the parsed circle intersects this line
		// resultLineCircle holds the result of the intersection test.
		bool intersectsCircle(const Vector2& vCirclePosition, float fCircleRadius);

	private:

	};

}