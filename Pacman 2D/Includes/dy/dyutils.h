#pragma once

typedef unsigned int	uint;

#include <cmath>

namespace dy
{
	bool static inline isEqual(float x, float y)
	{
		return fabs(x - y) < 0.1f;
	}

	bool static inline isInteger(float a)
	{
		return fabs(a - (int)a)<0.01f;
	}
}

