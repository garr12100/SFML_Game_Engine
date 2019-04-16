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


	void CCollider::HandleCollision(std::shared_ptr<CCollider> other, sf::Vector2f MTV, sf::Vector2f point, float deltaTime)
	{

		auto index = std::find(m_OverlappingWith.begin(), m_OverlappingWith.end(), other);
		if (index == m_OverlappingWith.end())
		{
			m_OverlappingWith.push_back(other);
			std::cout << "Collision Start: " << m_GameObject->name << " overlapping with " << other->m_GameObject->name << ". MTV = " << MTV.x << ", " << MTV.y << std::endl;// << returnOverlap << std::endl;
		}

		std::shared_ptr<CPhysics> physics = m_GameObject->GetComponentOfType<CPhysics>();
		std::shared_ptr<CPhysics> otherPhysics = other->m_GameObject->GetComponentOfType<CPhysics>();
		if (physics && otherPhysics)
		{
			if (!physics->GetStatic())
			{
				m_GameObject->GetTransform()->SetLocalPosition(m_GameObject->GetTransform()->GetLocalPosition() - MTV);

				physics->AddForce(-physics->GetMass() * physics->GetVelocity() / deltaTime, sf::Vector2f());
			}
			if (!otherPhysics->GetStatic())
			{
				other->GetGameObject()->GetTransform()->SetLocalPosition(other->GetGameObject()->GetTransform()->GetLocalPosition() + MTV);

				otherPhysics->AddForce(physics->GetMass() * physics->GetVelocity() / deltaTime, sf::Vector2f());
			}
		}
		else if (!isTrigger && !other->isTrigger)
		{
			if (!physics || !physics->GetStatic())
			{
				m_GameObject->GetTransform()->SetLocalPosition(m_GameObject->GetTransform()->GetLocalPosition() - MTV);
			}
			if (!otherPhysics || !otherPhysics->GetStatic())
			{
				other->GetGameObject()->GetTransform()->SetLocalPosition(other->GetGameObject()->GetTransform()->GetLocalPosition() + MTV);
			}
		}
		
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

	void CCollider::Start()
	{
		Reposition();
	}

	void CCollider::GetDebugOutline(sf::Vertex* debugOutline)
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
				if(collider != shared_from_base<CCollider>())
					CheckForCollision(collider, deltaTime);
			}
		}
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
		other->GetRect().GetAxes(axes+2);

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
					std::cout << "Collision End!" << m_GameObject->name << " overlapping with " << other->m_GameObject->name << std::endl;
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
	float CCollider::GetMomentOfInertia()
	{
		return std::powf(m_Rect.GetSize().x, 2.f) + std::powf(m_Rect.GetSize().y, 2.f) / 12.f;
	}
}