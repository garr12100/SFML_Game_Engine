#pragma once

#include "SFML/Graphics.hpp"
#include <math.h>
#include <limits>


namespace Utility
{
	const float MaxFloatValue = std::numeric_limits<float>::max();
	float Dot(const sf::Vector2f& a, const sf::Vector2f& b);
	float Cross(const sf::Vector2f& a, const sf::Vector2f& b);
	sf::Vector2f Cross(const sf::Vector2f& a, float s);
	sf::Vector2f Cross(float s, const sf::Vector2f& a);
	float Magnitude(const sf::Vector2f& v);
	sf::Vector2f Normalize(const sf::Vector2f& v);
	sf::Vector2f GetNormalVector(const sf::Vector2f& v);
	float Max(float* floatArray, int count);
	float Min(float* floatArray, int count);
	sf::Vector2f Average(std::vector<sf::Vector2f> vv);
	sf::Vertex& TransformVertex(const sf::Transform& t, sf::Vertex& v);
	std::vector<sf::Vertex> TransformVertices(const sf::Transform& t, std::vector<sf::Vertex> v);
	void TransformVertices(const sf::Transform& t, sf::Vertex* v, int count);
}