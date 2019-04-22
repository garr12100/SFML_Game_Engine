#include "CPhysics.h"
#include "GameObject.h"
#include "Transform.h"
#include "CCollider.h"
#include <iostream>
#include <math.h>
#include "Utility/MathUtility.h"

namespace Engine
{
	float CPhysics::gravity = -49.f;
	float CPhysics::dragCoefficient = .1f;
	float CPhysics::dragCoefficientAngular = 0.f;

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
			//m_Force *= (1 - dragCoefficient);
			//m_Torque *= (1 - dragCoefficientAngular);
			//m_Velocity *= (1 - (dragCoefficient * deltaTime));
			//m_AngularVelocity *= (1 - (dragCoefficientAngular * deltaTime));
		}
		sf::Vector2f linearAcceleration = m_Force / m_Mass;
		if (linearAcceleration.y > 0)
			std::cout << "Acceleration: " << linearAcceleration.y << std::endl;
		m_Velocity += linearAcceleration * deltaTime;
		//if (m_Velocity.y > 0)
			std::cout << "Velocity: " << m_Velocity.y << std::endl;
		m_GameObject->GetTransform()->Translate(m_Velocity * deltaTime);
		float angularAcceleration = m_Torque / (m_Mass * m_Collider->GetMomentOfInertia()); //MOMENT OF INERTIA!!
		m_AngularVelocity += angularAcceleration * deltaTime;
		if (m_GameObject->name == "Num 3" && angularAcceleration != 0)
			std::cout << "Angular Acc: " << angularAcceleration << std::endl;
		m_GameObject->GetTransform()->Rotate((m_AngularVelocity * deltaTime) * 180.f / 3.14159f);
		m_Force = sf::Vector2f(0.f, 0.f);
		m_Torque = 0.f;
	}
	void CPhysics::AddForce(sf::Vector2f force, sf::Vector2f position)
	{
		m_Force += force;
		sf::Vector2f c = m_Collider->GetRect().GetCenter();
		sf::Vector2f r = position - c;
		m_Torque += Utility::Cross(r, force);
		std::cout << "Force = " << m_Force.x << ", " << m_Force.y << std::endl;
		std::cout << "Torque = " << m_Torque << std::endl;
		
	}
}