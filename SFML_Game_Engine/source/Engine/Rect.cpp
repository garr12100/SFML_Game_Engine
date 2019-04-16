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

}
