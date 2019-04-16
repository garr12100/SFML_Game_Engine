#include "CPhysics.h"
#include "GameObject.h"
#include "Transform.h"
#include "CCollider.h"
#include <math.h>

namespace Engine
{
	float CPhysics::gravity = -9.8f;
	float CPhysics::dragCoefficient = .5f;

	CPhysics::CPhysics()
		:m_Static(false), m_HasGravity(true), m_HasDrag(true), m_Mass(1.f), m_AngularVelocity(0.f), m_Torque(0.f)
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

		if (m_HasGravity)
		{
			m_Force += sf::Vector2f(0, m_Mass * gravity);
		}
		if (m_HasDrag)
		{
			m_Force *= (1 - dragCoefficient);
			//m_Torque *= (1 - dragCoefficient);
			//m_Velocity.x *= (1 - (dragCoefficient * deltaTime));
		}
		sf::Vector2f linearAcceleration = sf::Vector2f(m_Force.x / m_Mass, m_Force.y / m_Mass);
		m_Velocity += linearAcceleration * deltaTime;
		m_GameObject->GetTransform()->Translate(m_Velocity * deltaTime);
		float angularAcceleration = m_Torque / m_Collider->GetMomentOfInertia(); //MOMENT OF INERTIA!!
		m_AngularVelocity += angularAcceleration * deltaTime;
		m_GameObject->GetTransform()->Rotate(m_AngularVelocity * deltaTime);
		m_Force = sf::Vector2f(0.f, 0.f);
	}
	void CPhysics::AddForce(sf::Vector2f force, sf::Vector2f position)
	{
		m_Force += force;
		sf::Vector2f c = m_Collider->GetRect().GetCenter();
		//sf::Vector2f r = position - c;
		//m_Torque += r.x * force.x + r.y * force.y;
	}
}