#include "CPhysics.h"
#include "GameObject.h"
#include "Transform.h"
#include "CCollider.h"
#include <iostream>
#include <math.h>
#include <memory>
#include "Utility/MathUtility.h"
#include "Raycast.h"

namespace Engine
{
	sf::Vector2f CPhysics::gravity = sf::Vector2f(0.f, -9.8f);
	float CPhysics::dragCoefficient = .1f;
	float CPhysics::dragCoefficientAngular = .1f;

	CPhysics::CPhysics()
		:m_Static(false), m_HasGravity(true), m_HasDrag(true), m_Mass(1.f), m_Bounciness(.75f),
		m_Grounded(false), m_AngularVelocity(0.f), m_Torque(0.f), m_GroundDistanceIndex(0)
	{
	}

	void CPhysics::Start()
	{
		if (!m_Collider)
			m_Collider = m_GameObject->GetComponentOfType<CCollider>();
	}

	void CPhysics::PhysicsUpdate(float deltaTime)
	{
		if (m_Static || !m_Collider)
			return;

		if (m_Grounded)
		{
			if (!(m_Force.x == 0.f && m_Force.y == 0.f))
				m_Grounded = false;
		}

		if (m_HasGravity)
		{
			//Apply gravity, then see if grounded. 
			m_Force += m_Mass * gravity;
			
		}
		if (m_HasDrag)
		{
			m_Velocity *= (1 - (dragCoefficient * deltaTime));
			m_AngularVelocity *= (1 - (dragCoefficientAngular * deltaTime));
		}


		sf::Vector2f linearAcceleration = m_Force / m_Mass;
		m_Velocity += linearAcceleration * deltaTime;
		if (m_Grounded)
			m_Velocity.y = 0.f;
		m_GameObject->GetTransform()->Translate(m_Velocity * deltaTime);
		float angularAcceleration = m_Torque / (m_Mass * m_Collider->GetMomentOfInertia()); //MOMENT OF INERTIA!!
		m_AngularVelocity += angularAcceleration * deltaTime;
		m_GameObject->GetTransform()->Rotate((m_AngularVelocity * deltaTime) * 180.f / 3.14159f);
		m_Force = sf::Vector2f(0.f, 0.f);
		m_Torque = 0.f;

		if (m_HasGravity)
		{
			float rectLength = m_Collider->GetRect().GetSize().x > m_Collider->GetRect().GetSize().y ? m_Collider->GetRect().GetSize().x : m_Collider->GetRect().GetSize().y;
			RaycastHit selfHit;
			if (Raycast::PerformRaycast(m_Collider->GetRect().GetCenter(), Utility::Normalize(gravity), rectLength, selfHit, nullptr, false))
			{
				float l = Utility::Distance(selfHit.point, m_Collider->GetRect().GetCenter()) + .1f;
				RaycastHit groundHit;
				CCollider* col = m_Collider.get();
				if (Raycast::PerformRaycast(m_Collider->GetRect().GetCenter(), Utility::Normalize(gravity), l, groundHit, col))
				{
					m_GroundDistances[m_GroundDistanceIndex] = 1;
				}
				else
					m_GroundDistances[m_GroundDistanceIndex] = 0;

				m_GroundDistanceIndex = (m_GroundDistanceIndex + 1) % 30;

				//Use running average to see if we're touching the ground. 
				if (Utility::Average(m_GroundDistances, 30) > .9f)
					m_Grounded = true;
				else
					m_Grounded = false;
			}
		}

	}
	void CPhysics::AddForce(const sf::Vector2f& force, const sf::Vector2f& position)
	{
		m_Force += force;
		sf::Vector2f c = m_Collider->GetRect().GetCenter();
		sf::Vector2f r = position - c;
		m_Torque += Utility::Cross(r, force);		
	}
}