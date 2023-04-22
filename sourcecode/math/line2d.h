#pragma once
#include "../precompiled_header.h"
#include "vector2.h"

namespace Nexus
{

	// Used by CLine2D to hold intersection results for line-line
	class Line2DIntersectionResultLineLine
	{
	public:
		Line2DIntersectionResultLineLine();

		bool bIntersectionOccurred;	// Whether there was an intersection or not
		Vector2 vIntersectionPoint;	// If bIntersection is true, this holds the point of intersection
	};

	// Used by CLine2D to hold intersection results for line-circle
	class Line2DIntersectionResultLineCircle
	{
	public:
		Line2DIntersectionResultLineCircle();

		bool bInside;			// Is true if both line points are inside the circle
		bool bIntersects;		// Is true if at least one intersection point was found
		bool bTangent;			// Is true if only one intersection point was found (Use bEnterComputed and bExitComputed to determine which)
		bool bEnterComputed;	// vEnter intersection point has been found
		bool bExitComputed;		// vExit intersection point has been found
		Vector2 vEnter;			// Holds "enter" intersection point (if vEnterComputed == true)
		Vector2 vExit;			// Holds "exit" intersection point (if vExitComputed == true)

	};

	// A class which represents a 2D line
	// Usefull for 2D line intersection tests and circle line intersection tests
	class Line2D
	{
	public:
		Line2D();

		// Construct the line
		Line2D(const Vector2& vBeginPos, const Vector2& vEndPos);

		Vector2 begin;					// Beginning point
		Vector2 end;					// End point

		Line2DIntersectionResultLineLine resultLineLine;		// Will hold intersection test result information for line/line intersection test
		Line2DIntersectionResultLineCircle resultLineCircle;	// Will hold intersection test result information for line/circle intersection test

		// Returns true if the given point is "left" of this line, else false
		bool isPointLeft(Vector2 vPoint);

		// Returns true if the parsed line intersects this one
		// If true is returned, intersectionResult member holds information about the intersection
		bool intersectionTest(const Line2D& otherLine);

		// Returns true if the parsed circle intersects this line
		// resultLineCircle holds the result of the intersection test.
		bool intersectsCircle(const Vector2& vCirclePosition, float fCircleRadius);

	private:

	};

}