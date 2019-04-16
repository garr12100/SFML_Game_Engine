#include "MathUtility.h"

namespace Utility
{
	float Dot(sf::Vector2f a, sf::Vector2f b)
	{
		return a.x * b.x + a.y * b.y;
	}

	float Magnitude(sf::Vector2f v)
	{
		return std::sqrtf(std::pow(v.x, 2) + std::pow(v.y, 2));
	}

	sf::Vector2f Normalize(sf::Vector2f v)
	{
		return v / Magnitude(v);
	}

	float Max(float * floatArray, int count)
	{
		if (count == 0)
			return 0.f;
		float max = -std::numeric_limits<float>::max();
		for (int i = 0; i < count; i++)
		{
			if (floatArray[i] > max)
				max = floatArray[i];
		}
		return max;
	}

	float Min(float * floatArray, int count)
	{
		if (count == 0)
			return 0.f;
		float min = std::numeric_limits<float>::max();
		for (int i = 0; i < count; i++)
		{
			if (floatArray[i] < min)
				min = floatArray[i];
		}
		return min;
	}
}