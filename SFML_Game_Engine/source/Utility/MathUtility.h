#pragma once

#include "SFML/Graphics.hpp"
#include <math.h>
#include <limits>


namespace Utility
{
	const float MaxFloatValue = std::numeric_limits<float>::max();
	float Dot(sf::Vector2f a, sf::Vector2f b);
	float Magnitude(sf::Vector2f v);
	sf::Vector2f Normalize(sf::Vector2f v);

	float Max(float* floatArray, int count);
	float Min(float* floatArray, int count);
}