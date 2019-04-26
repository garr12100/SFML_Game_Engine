#include "Raycast.h"
#include "GameObject.h"
#include "Components/CCollider.h"
#include "Utility/MathUtility.h"
#include <iostream>

namespace Engine
{
	// Find Intersection between 2 lines, if there is one. Populates hitPoint and normal if a collision has occured. 
	static bool FindIntersection(const LineSegment& a, const LineSegment& b, sf::Vector2f& hitPoint, sf::Vector2f& normal)
	{
		sf::Vector2f r = a.end - a.start;
		sf::Vector2f s = b.end - b.start;
		float rCrossS = Utility::Cross(r, s);
		float t = Utility::Cross((b.start - a.start), s) / rCrossS;
		float u = Utility::Cross((a.start - b.start), r) / -rCrossS;
		if (t > 0.f && t < 1.f && u > 0.f && t < 1.f)
		{
			//They intersect! 
			hitPoint = a.start + (r * t);
			normal = Utility::GetOrthogonalVector(s);
			if (Utility::Dot(normal, r) > 0.f)
				normal = -normal;
			return true;
		}
		return false;
	}

	std::vector<std::vector<sf::Vertex>> Raycast::m_CurrentRaycasts;
	bool Raycast::debug = true;

	bool Raycast::PerformRaycast(const sf::Vector2f& startPosition, sf::Vector2f direction, float length, RaycastHit& hit, CCollider* ignore, bool debuggable)
	{
		bool hasHit = false;
		sf::Vector2f endPosition = startPosition + (Utility::Normalize(direction) * length);
		if (debug && debuggable)
		{
			std::vector<sf::Vertex> verts;
			verts.push_back(sf::Vertex(startPosition, sf::Color::Cyan));
			verts.push_back(sf::Vertex(endPosition, sf::Color::Cyan));
			m_CurrentRaycasts.push_back(verts);
		}

		LineSegment ray = LineSegment(startPosition, endPosition);
		for (std::shared_ptr<GameObject> go : GameObject::GetAllGameObjects())
		{
			std::vector<std::shared_ptr<Engine::CCollider>> colliders = go->GetComponentsOfType<Engine::CCollider>();
			for (std::shared_ptr<Engine::CCollider> col : colliders)
			{
				if (col.get() == ignore)
					continue;
				sf::Vector2f corners[4];
				col->GetRect().GetCorners(corners);
				LineSegment edges[4] = {
					LineSegment(corners[0], corners[1]),
					LineSegment(corners[1], corners[2]),
					LineSegment(corners[2], corners[3]),
					LineSegment(corners[3], corners[0])
				};
				for (LineSegment edge : edges)
				{
					sf::Vector2f intersectionPoint;
					sf::Vector2f intersectionNormal;
					if (FindIntersection(ray, edge, intersectionPoint, intersectionNormal))
					{
						if (!hasHit)
						{
							hit.point = intersectionPoint;
							hit.collider = col;
							hit.normal = intersectionNormal;
							hasHit = true;
						}
						else if (Utility::Distance(startPosition, intersectionPoint) < Utility::Distance(startPosition, hit.point))
						{
							hit.point = intersectionPoint;
							hit.normal = intersectionNormal;
							hit.collider = col;
						}
					}
				}
			}
		}
		if (hasHit)
			return true;
		else
			return false;
	}

	std::vector<std::vector<sf::Vertex>> Raycast::GetCurrentRaycasts()
	{
		return m_CurrentRaycasts;
	}

	void Raycast::ClearCurrentRaycasts()
	{
		m_CurrentRaycasts.clear();
	}

	LineSegment::LineSegment(sf::Vector2f s, sf::Vector2f e)
		: start(s), end(e)
	{
	}

}