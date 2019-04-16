#pragma once
#include "Component.h"
#include "SFML/Graphics.hpp"

namespace Engine
{
	class CPhysics : public Component
	{
	private:
		bool m_Static;
		bool m_HasGravity;
		bool m_HasDrag;
		float m_Mass;
		sf::Vector2f m_Velocity;
		float m_AngularVelocity;
		sf::Vector2f m_Force;
		float m_Torque;
		std::shared_ptr<class CCollider> m_Collider;
	public:
		static float gravity;
		static float dragCoefficient;
		CPhysics();
		virtual void Start() override;
		virtual void PhysicsUpdate(float deltaTime);

		void AddForce(sf::Vector2f force, sf::Vector2f position);

		inline bool GetStatic() { return m_Static; }
		inline void SetStatic(bool val) { m_Static = val; }

		inline bool GetHasGravity() { return m_HasGravity; }
		inline void SetHasGravity(bool val) { m_HasGravity = val; }
		
		inline bool GetHasDrag() { return m_HasDrag; }
		inline void SetHasDrag(bool val) { m_HasDrag = val; }

		inline float GetMass() { return m_Mass; }
		inline void SetMass(float val) { m_Mass = val; }

		inline sf::Vector2f GetVelocity() { return m_Velocity; }
		inline void SetVelocity(sf::Vector2f val) { m_Velocity = val; }

	};
}