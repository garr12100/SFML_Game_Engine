#include "CCollider.h"
#include "CPhysics.h"
#include "GameObject.h"
#include "Transform.h"
#include <math.h>
#include "Utility/MathUtility.h"
#include <vector>
#include <iostream>

namespace Engine
{

	bool CCollider::debug = false;


	void CCollider::HandleCollision(std::shared_ptr<CCollider> other, const sf::Vector2f& MTV, const sf::Vector2f& point, float deltaTime)
	{
		//If MTV too small, do nothing: 
		if (Utility::Magnitude(MTV) < .001)
			return;
		bool newCollision = false;
		//Check to see if this collision already existed previous frame:
		auto index = std::find(m_OverlappingWith.begin(), m_OverlappingWith.end(), other);
		if (index == m_OverlappingWith.end())
		{
			newCollision = true;
			m_OverlappingWith.push_back(other);
			//std::cout << "Collision Start: " << m_GameObject->name << " overlapping with " << other->m_GameObject->name << ". MTV = " << MTV.x << ", " << MTV.y << std::endl;// << returnOverlap << std::endl;
			//std::cout << "dT = " << deltaTime << std::endl;
		}

		std::shared_ptr<CPhysics> physics = m_GameObject->GetComponentOfType<CPhysics>();
		std::shared_ptr<CPhysics> otherPhysics = other->m_GameObject->GetComponentOfType<CPhysics>();
		if (isTrigger || other->isTrigger)
		{
			//Fire trigger event, don't resolve collsision
		}
		else if (newCollision && physics && otherPhysics)
		{
			//Physics based collision resolution
			std::vector<sf::Vector2f> contactPoints = FindContactPoint(other, Utility::Normalize(MTV));
			if (contactPoints.size() > 0)
			{
				sf::Vector2f averageContactPoint = Utility::Average(contactPoints);
				for (sf::Vector2f contactPoint : contactPoints)
				{
					contactVerts.push_back(sf::Vertex(contactPoint, sf::Color::Cyan));
				}
				contactVerts.push_back(sf::Vertex(averageContactPoint, sf::Color::Blue));

				//Transform the collision point to the collider's previous Transform to get an estimate of impact velocity: 
				sf::Vector2f inversePoint = m_GameObject->GetTransform()->GetMatrix().getInverse().transformPoint(averageContactPoint);
				sf::Vector2f previousContactPoint = m_PreviousTransform.transformPoint(inversePoint);
				sf::Vector2f impactVelocity = averageContactPoint - previousContactPoint;
				float impactVelocityMagnitude = Utility::Magnitude(impactVelocity);
				sf::Vector2f impactVelocitytDir = Utility::Normalize(impactVelocity);
				sf::Vector2f dirToOther = Utility::Normalize(m_GameObject->GetTransform()->GetPosition() - other->m_GameObject->GetTransform()->GetPosition());
				//Orient magnitude of impact Velocity. 
				if (Utility::Dot(impactVelocitytDir, dirToOther) > 0.f)
					impactVelocityMagnitude *= -1.f;
				//Apply bounciness of physics object: 
				impactVelocityMagnitude *= physics->GetBounciness();

				//Find the impact velocity of the Other collider: 
				sf::Vector2f other_inversePoint = other->m_GameObject->GetTransform()->GetMatrix().getInverse().transformPoint(averageContactPoint);
				sf::Vector2f other_previousContactPoint = other->m_PreviousTransform.transformPoint(other_inversePoint);
				sf::Vector2f otherImpactVelocity = averageContactPoint - other_previousContactPoint;
				float otherImpactVelocityMagnitude = Utility::Magnitude(otherImpactVelocity);
				sf::Vector2f otherImpactVelocitytDir = Utility::Normalize(otherImpactVelocity);
				sf::Vector2f dirToThis = -dirToOther;
				//Orient magnitude of impact Velocity. 
				if (Utility::Dot(otherImpactVelocitytDir, dirToThis) < 0.f)
					otherImpactVelocityMagnitude *= -1.f;
				otherImpactVelocityMagnitude *= otherPhysics->GetBounciness();


				//Get Delta Positions, masses, and calculate force to apply: 
				sf::Vector2f dp1 =  impactVelocityMagnitude * Utility::Normalize(MTV);
				sf::Vector2f dp2 =  otherImpactVelocityMagnitude * Utility::Normalize(MTV);
				float m1 = physics->GetMass();
				float m2 = otherPhysics->GetMass();
				sf::Vector2f force = m2 * (((2 * m1 * dp1) + ((m2 - m1) * dp2)) / (deltaTime * deltaTime * (m1 + m2))) - ((m2 * dp2) / (deltaTime * deltaTime));

				//Apply force at point:
				if (contactPoints.size() > 0 && !std::isnan(force.x) && !std::isnan(force.y))
					otherPhysics->AddForce(force, averageContactPoint);

			}
		}
		else
		{
			//Non physics based collision resolution
			if ( (!physics || !physics->GetStatic()))
			{
				m_GameObject->GetTransform()->SetLocalPosition(m_GameObject->GetTransform()->GetLocalPosition() - MTV);
			}
		}
		Reposition();
	}

	void CCollider::Reposition()
	{
		sf::Vector2f position = m_GameObject->GetTransform()->GetPosition() + offset;
		float rotation = m_GameObject->GetTransform()->GetRotation();
		sf::Vector2f scale = m_GameObject->GetTransform()->GetScale();
		scale.x *= sizeOffset.x;
		scale.y *= sizeOffset.y;
		m_Rect.Set(position, rotation, scale);
	}

	std::vector<sf::Vector2f> CCollider::FindContactPoint(std::shared_ptr <CCollider> other, const sf::Vector2f& dir)
	{
		bestEdges.clear();
		bool flip = false;
		Edge e1 = m_Rect.GetBestEdge(dir);
		Edge e2 = other->m_Rect.GetBestEdge(-dir);
		Edge ref, inc;
		if (std::abs(Utility::Dot(e1.GetVector(), dir)) <= std::abs(Utility::Dot(e2.GetVector(), dir)))
		{
			ref = e1;
			inc = e2;
		}
		else
		{
			ref = e2;
			inc = e1;
			flip = true;
		}
		std::vector<sf::Vertex> be1, be2;
		be1.push_back(sf::Vertex(e1.v0, sf::Color::Red));
		be1.push_back(sf::Vertex(e1.v1, sf::Color::Red));

		be2.push_back(sf::Vertex(e2.v0, sf::Color::Red));
		be2.push_back(sf::Vertex(e2.v1, sf::Color::Red));

		bestEdges.push_back(be1);
		bestEdges.push_back(be2);

		sf::Vector2f refV = Utility::Normalize(ref.GetVector());
		float o1 = Utility::Dot(refV, ref.v0);
		//Clip the incident ege by the first vertex of the reference edge
		std::vector<sf::Vector2f> clippedPoints = Clip(inc.v0, inc.v1, refV, o1);
		//if we don't have 2 points left, then fail
		if (clippedPoints.size() < 2)
			return std::vector<sf::Vector2f>();

		//Clip what's left of the incident edge by the second vertex of the 
		//reference edge.  but we ned to cli the opposite direction so we flip 
		//the direction and offset. 
		float o2 = Utility::Dot(refV, ref.v1);
		clippedPoints = Clip(clippedPoints[0], clippedPoints[1], -refV, -o2);
		//if we don't have 2 points left, then fail
		if (clippedPoints.size() < 2)
			return std::vector<sf::Vector2f>();

		//Get the reference edge normal
		sf::Vector2f refNorm = Utility::GetOrthogonalVector(ref.GetVector());
		//if we had to flip the incident abnd ref edges then we need to flip
		//the reference edge normal to clip properly. 
		if (flip)
			refNorm *= -1.f;
		//Get the largest depth: 
		float max = Utility::Dot(refNorm, ref.maximumProjectionVertex);


		//Make sure the final points are not past this maximum. 

		if (!flip) {
			if (Utility::Dot(refNorm, clippedPoints[1]) > max)
				clippedPoints.erase(clippedPoints.begin() + 1);

			if (Utility::Dot(refNorm, clippedPoints[0]) > max)
				clippedPoints.erase(clippedPoints.begin());
		}
		else {
			if (Utility::Dot(refNorm, clippedPoints[1]) < max)
				clippedPoints.erase(clippedPoints.begin() + 1);

			if (Utility::Dot(refNorm, clippedPoints[0]) < max)
				clippedPoints.erase(clippedPoints.begin());
		}

		return clippedPoints;
	}

	std::vector<sf::Vector2f> CCollider::Clip(const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& n, float o)
	{
		//clip the line segment points v1, v2 if they are past 0 along n
		std::vector<sf::Vector2f> clippedPoints;
		float d1 = Utility::Dot(n, v1) - o;
		float d2 = Utility::Dot(n, v2) - o;
		//if either point is past o along n then we can keep the point
		if (d1 >= 0.f) clippedPoints.push_back(v1);
		if (d2 >= 0.f) clippedPoints.push_back(v2);
		//finally we need to check if they are on opposing sides so that
		//we can correctly compute the correct point
		if (d1 * d2 < 0.f)
		{
			//If they are on different sides of the offset, d1 and d2 will be a
			//(+) * (-) and will yield a (-)
			sf::Vector2f e = v2 - v1;
			float u = d1 / (d1 - d2);
			e *= u;
			e += v1;
			clippedPoints.push_back(e);
		}
		return clippedPoints;
	}

	void CCollider::Start()
	{
		Reposition();
	}

	void CCollider::GetDebugOutline(sf::Vertex* debugOutline) const
	{
		//Start with top left point, go clockwise: 
		sf::Vector2f corners[4];
		m_Rect.GetCorners(corners);
		debugOutline[0].position = corners[0];
		debugOutline[1].position = corners[1];
		debugOutline[2].position = corners[2];
		debugOutline[3].position = corners[3];
		debugOutline[4].position = corners[0];
		debugOutline[0].color = sf::Color(25, 200, 75);
		debugOutline[1].color = sf::Color(25, 200, 75);
		debugOutline[2].color = sf::Color(25, 200, 75);
		debugOutline[3].color = sf::Color(25, 200, 75);
		debugOutline[4].color = sf::Color(25, 200, 75);
	}

	void CCollider::CheckForCollisions(float deltaTime)
	{
		//Update all objects, then calculate physics/collision
		for (std::shared_ptr<GameObject> go : GameObject::GetAllGameObjects())
		{
			std::vector<std::shared_ptr<CCollider>> colliders = go->GetComponentsOfType<CCollider>();
			for (std::shared_ptr<CCollider> collider : colliders)
			{
				if (collider != shared_from_base<CCollider>())
					CheckForCollision(collider, deltaTime);
			}
		}

	}

	void CCollider::ResetPreviousTransform()
	{
		m_PreviousTransform = m_GameObject->GetTransform()->GetMatrix();
	}

	void CCollider::CheckForCollision(std::shared_ptr<CCollider> other, float deltaTime)
	{
		Reposition();
		sf::Vector2f corners1[4];
		m_Rect.GetCorners(corners1);
		sf::Vector2f axes[4];
		m_Rect.GetAxes(axes);

		sf::Vector2f corners2[4];
		other->GetRect().GetCorners(corners2);
		//sf::Vector2f axes2[2];
		other->GetRect().GetAxes(axes + 2);

		//std::vector<sf::Vector2f> MTVs;

		sf::Vector2f MTV;
		float MTVMag = Utility::MaxFloatValue;
		int intersectionCount = 0;
		sf::Vector2f intersectionPoint;
		for (int i = 0; i < 4; i++)
		{
			float scalars1[4], scalars2[4];

			for (int j = 0; j < 4; j++)
			{
				scalars1[j] = Utility::Dot(axes[i], corners1[j]);
				scalars2[j] = Utility::Dot(axes[i], corners2[j]);
			}
			float s1Max = Utility::Max(scalars1, 4);
			float s1Min = Utility::Min(scalars1, 4);
			float s2Max = Utility::Max(scalars2, 4);
			float s2Min = Utility::Min(scalars2, 4);

			if (s2Min > s1Max || s2Max < s1Min)
			{
				auto index = std::find(m_OverlappingWith.begin(), m_OverlappingWith.end(), other);
				if (index != m_OverlappingWith.end())
				{
					m_OverlappingWith.erase(index);
					//std::cout << "Collision End!" << m_GameObject->name << " overlapping with " << other->m_GameObject->name << std::endl;
				}
				return;
			}
			//There was an intersection! 

			float overlap = s1Max > s2Max ? -(s2Max - s1Min) : s1Max - s2Min;

			sf::Vector2f newMTV = overlap * axes[i];
			if (Utility::Magnitude(newMTV) < MTVMag)
			{
				MTV = newMTV;
				MTVMag = Utility::Magnitude(MTV);
			}
		}

		HandleCollision(other, MTV, sf::Vector2f(), deltaTime);

	}
	float CCollider::GetMomentOfInertia() const
	{
		return std::powf(m_Rect.GetSize().x, 2.f) + std::powf(m_Rect.GetSize().y, 2.f) / 12.f;
	}


}