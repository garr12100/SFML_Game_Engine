#include "MathUtility.h"

namespace Utility
{
	float Distance(const sf::Vector2f & a, const sf::Vector2f & b)
	{
		return Magnitude(b - a);
	}

	float Dot(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	float Cross(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	sf::Vector2f Cross(const sf::Vector2f& a, float s)
	{
		return sf::Vector2f(s * a.y, -s * a.x);
	}

	sf::Vector2f Cross(float s, const sf::Vector2f& a)
	{
		return sf::Vector2f(-s * a.y, s * a.x);
	}

	float Magnitude(const sf::Vector2f& v)
	{
		return std::sqrtf(std::pow(v.x, 2) + std::pow(v.y, 2));
	}

	sf::Vector2f Normalize(const sf::Vector2f& v)
	{
		return v / Magnitude(v);
	}

	sf::Vector2f GetOrthogonalVector(const sf::Vector2f& v)
	{
		return Normalize(sf::Vector2f(-v.y, v.x));
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
	sf::Vector2f Average(std::vector<sf::Vector2f> vv)
	{
		if (vv.size() == 0)
			return sf::Vector2f();
		float avgX = 0.f;
		float avgY = 0.f;
		for (sf::Vector2f v : vv)
		{
			avgX += v.x;
			avgY += v.y;
		}
		sf::Vector2f average(avgX, avgY);
		
		average = average / (float)vv.size();
		
		return average;
	}

	float Average(float * a, int count)
	{
		if (count == 0)
			return 0.f;
		float average = 0;
		for (int i = 0; i < count; i++)
		{
			average += a[i];
		}

		average /= (float)count;

		return average;
	}

	sf::Vertex& TransformVertex(const sf::Transform & t, sf::Vertex & v)
	{
		v.position = t.transformPoint(v.position);
		return v;
	}

	std::vector<sf::Vertex> TransformVertices(const sf::Transform & t, std::vector<sf::Vertex> v)
	{
		for (unsigned int i = 0; i < v.size(); i++)
		{
			v[i] = TransformVertex(t, v[i]);
		}
		return v;
	}
	void TransformVertices(const sf::Transform & t, sf::Vertex * v, int count)
	{
		for (int i = 0; i < count; i++)
		{
			TransformVertex(t, v[i]);
		}
	}
}