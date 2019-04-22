#include "Rect.h"
#include "Utility/MathUtility.h"


namespace Engine
{
	Rect::Rect()
		: m_Center(0, 0), m_Width(0), m_Height(0)
	{
	}
	
	Rect::Rect(float x, float y, float w, float h)
		: m_Center(x, y), m_Width(w), m_Height(h)
	{
	}

	void Rect::GetCorners(sf::Vector2f* corners)
	{
		sf::Transform matrix;
		matrix.translate(m_Center).rotate(m_Rotation).scale(GetSize());
		//Starts top left, goes clockwise
		corners[0] = matrix.transformPoint(-0.5f, 0.5f);
		corners[1] = matrix.transformPoint(0.5f, 0.5f);
		corners[2] = matrix.transformPoint(0.5f, -0.5f);
		corners[3] = matrix.transformPoint(-0.5f, -0.5f);
	}

	void Rect::GetAxes(sf::Vector2f* axes)
	{
		sf::Vector2f corners[4]; 
		GetCorners(corners);
		//Axis Left to Right
		axes[0] = Utility::Normalize(corners[0] - corners[1]);
		//Axis Bottom to Top
		axes[1] = Utility::Normalize(corners[2] - corners[1]);
	}

	Edge Rect::GetBestEdge(sf::Vector2f dir)
	{
		//Find the furthest Vertex in the rect along the direction:
		sf::Vector2f corners[4];
		GetCorners(corners);
		float max = -Utility::MaxFloatValue;
		int maxIndex = 0;
		for (int i = 0; i < 4; i++)
		{
			float projection = Utility::Dot(dir, corners[i]);
			if (projection > max)
			{
				max = projection;
				maxIndex = i;
			}
		}

		//Now find the edge that is most perpindicular, left or right: 
		sf::Vector2f vert = corners[maxIndex];
		sf::Vector2f v1 = corners[(maxIndex + 1) % 4];
		sf::Vector2f v0 = corners[((maxIndex - 1) + 4) % 4];
		sf::Vector2f left = vert - v1;
		sf::Vector2f right = vert - v0;
		left = Utility::Normalize(left);
		right = Utility::Normalize(right);
		if (Utility::Dot(right, dir) <= Utility::Dot(left, dir))
		{
			return Edge(vert, v0, vert);
		}
		else
		{
			return Edge(vert, vert, v1);
		}
	}



	Edge::Edge(sf::Vector2f max, sf::Vector2f _v0, sf::Vector2f _v1)
		: maximumProjectionVertex(max), v0(_v0), v1(_v1)
	{
	}

	Edge::Edge()
	{
	}

}
