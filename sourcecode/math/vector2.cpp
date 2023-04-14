#include "precompiled_header.h"
#include "vector2.h"
#include "../log.h"

namespace Nexus
{

	inline const Vector2 Vector2::operator /(const float f) const
	{
		if (f == 0)
			Log::getPointer()->addException("Vector2::operator / passed a scalar of zero");
		float	reciprocal = 1.0f / f;
		return Vector2(x * reciprocal, y * reciprocal);
	}

}
