#include "precompiled_header.h"
#include "vector3.h"
#include "../core/log.h"

namespace Nexus
{

	inline const Vector3 Vector3::operator /(const float f) const
	{
		if (f == 0)
			Log::getPointer()->exception("Vector3::operator / passed a scalar of zero");
		float	reciprocal = 1.0f / f;
		return Vector3(x * reciprocal, y * reciprocal, z * reciprocal);
	}
}
