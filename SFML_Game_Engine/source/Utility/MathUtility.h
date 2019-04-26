#pragma once

#include "SFML/Graphics.hpp"
#include <math.h>
#include <limits>

// Namespace for various math/vector utilities. May be replaced by an open source Math library at some point. 
namespace Utility
{
	// Maximum value of a float 
	const float MaxFloatValue = std::numeric_limits<float>::max();

	// Get distance between 2 points. 
	float Distance(const sf::Vector2f& a, const sf::Vector2f& b);

	// Get dot product of 2 vectors. 
	float Dot(const sf::Vector2f& a, const sf::Vector2f& b);

	// Get cross product of 2 vectors. 
	float Cross(const sf::Vector2f& a, const sf::Vector2f& b);

	// Get cross product of a vector and a scalar. 
	sf::Vector2f Cross(const sf::Vector2f& a, float s);

	// Get cross product of a scalar and a vector. 
	sf::Vector2f Cross(float s, const sf::Vector2f& a);

	// Get magnitude of a vector. 
	float Magnitude(const sf::Vector2f& v);

	// Get normalized value of a vector. 
	sf::Vector2f Normalize(const sf::Vector2f& v);

	// Get a Vector orthogonal to a vector. 
	sf::Vector2f GetOrthogonalVector(const sf::Vector2f& v);

	// Get Max value from a float array. 
	float Max(float* floatArray, int count);

	//Get Minimum value from a float array. 
	float Min(float* floatArray, int count);

	// Get Average vector value from a list of vectors. 
	sf::Vector2f Average(std::vector<sf::Vector2f> vv);

	// Get average value of a float array. 
	float Average(float* a, int count);

	//Transform a Vertex by a transformation matrix. 
	sf::Vertex& TransformVertex(const sf::Transform& t, sf::Vertex& v);

	// Transform a list of verticies by a transformation matrix. 
	std::vector<sf::Vertex> TransformVertices(const sf::Transform& t, std::vector<sf::Vertex> v);

	// Transform an array of vertices by a transformation matrix. 
	void TransformVertices(const sf::Transform& t, sf::Vertex* v, int count);
}