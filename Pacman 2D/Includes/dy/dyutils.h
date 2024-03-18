#pragma once

typedef unsigned int	uint;

#include <cmath>
#include <random>

namespace dy
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	bool static inline isEqual(float x, float y)
	{
		return fabs(x - y) < 0.1f;
	}

	bool static inline isInteger(float a)
	{
		return fabs(a - (int)a)<0.1f;
	}

	int static inline ranInt(int min, int max)
	{
		std::uniform_int_distribution<int> dis(min, max);
		return dis(gen);
	}
}

