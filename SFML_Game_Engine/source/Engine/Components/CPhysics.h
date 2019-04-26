#pragma once
#include "Component.h"
#include "SFML/Graphics.hpp"

namespace Engine
{
	//Physics component - applies physics to its owning GameObject. Requires a Collider Component to work. 
	class CPhysics : public Component
	{
	private:
		// Static - Object has infinite mass, cannot be moved. 
		bool m_Static;

		// Does gravity affect this object? Default is True. 
		bool m_HasGravity;

		// Does drag affect this object? Default is True.
		bool m_HasDrag;

		// Mass of the object. 
		float m_Mass;

		// Bounciness of the object. 
		float m_Bounciness;

		// Is the object touching the ground? 
		bool m_Grounded;

		// Current Linear Velocity.
		sf::Vector2f m_Velocity;

		// Current Angular Velocity. 
		float m_AngularVelocity;

		// Total Forces applied this frame.
		sf::Vector2f m_Force;

		// Total Torque applied this frame. 
		float m_Torque;

		// Reference to Collider attached to this game object. 
		std::shared_ptr<class CCollider> m_Collider;

		// Each frame, a Raycast will be fired from the bottom of the object down to determine if it is on the ground. 
		// This array keeps track of that state and is used to compute a running average to filter out instantaneous collisions and allow bouncing. 
		float m_GroundDistances[30] = { 0 };

		// Current index for the ground distance array. Increments each from and wraps around when it reaches the end. 
		int m_GroundDistanceIndex;

	public:
		// Global gravity acceleration. 
		static sf::Vector2f gravity;

		// Global linear drag - air resitance.
		static float dragCoefficient;

		// Global  angular drag. 
		static float dragCoefficientAngular;

		CPhysics();
		virtual void Start() override;

		// Update called with fixed delta time value to compute physics. 
		virtual void PhysicsUpdate(float deltaTime);

		// Add a force to the object at a specific position. It is recommended that the position is contained by the object. 
		void AddForce(const sf::Vector2f& force, const sf::Vector2f& position);

		inline bool GetStatic() const { return m_Static; }
		inline void SetStatic(bool val) { m_Static = val; }

		inline bool GetHasGravity() const { return m_HasGravity; }
		inline void SetHasGravity(bool val) { m_HasGravity = val; }
		
		inline bool GetHasDrag() const { return m_HasDrag; }
		inline void SetHasDrag(bool val) { m_HasDrag = val; }

		inline float GetMass() const { return m_Static ? 1E08f : m_Mass; }
		inline void SetMass(float val) { m_Mass = val; }
		
		inline float GetBounciness() const { return m_Bounciness; }
		inline void SetBounciness(float val) { m_Bounciness = val; }

		inline sf::Vector2f GetVelocity() const { return m_Velocity; }
		inline void SetVelocity(const sf::Vector2f& val) { m_Velocity = val; }

		inline float GetAngularVelocity() const { return m_AngularVelocity; }
		inline void SetAngularVelocity(float val) { m_AngularVelocity = val; }

	};
}