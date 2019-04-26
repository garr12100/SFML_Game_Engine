#pragma once
#include "Components/Component.h"
#include "Rect.h"
#include <memory>
#include <vector>

namespace Engine
{
	// Collider component. Only rectangle shaped, for now. 
	class CCollider : public Component
	{
	private:
		// Rect that defines the collider
		Rect m_Rect;

		// Vector of other Colliders overlapping with this one. 
		std::vector<std::shared_ptr<CCollider>> m_OverlappingWith;

		// Resolve collision/apply updates based on presence of Physics component, Collider parameters, etc. TODO: Add delegates for Trigger/Collision events
		void HandleCollision(std::shared_ptr<CCollider> other, const sf::Vector2f& MTV, const sf::Vector2f& point, float deltaTime);

		// Reposition the Rect based on Transform. 
		void Reposition();

		// Find contact points of collision using Clipping algorithm. 
		std::vector<sf::Vector2f> FindContactPoint(std::shared_ptr <CCollider> other, const sf::Vector2f& dir);

		// Helper function for Clipping algorithm. 
		std::vector<sf::Vector2f> Clip(const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& n, float o);

		// Cached Transform matrix from previous frame. 
		sf::Transform m_PreviousTransform;

	public:
		// Debug rectangle outline and collision data?
		static bool debug;

		// Is this a trigger collider?
		bool isTrigger = false;

		// Vertices of contact points. Used for debugging.
		std::vector<sf::Vertex> contactVerts;

		// Best edges during collision. Used for debugging.
		std::vector<std::vector<sf::Vertex>> bestEdges;

		// Position offset of the collider from its GameObject
		sf::Vector2f offset;

		// Size offset of the collider from its GameObject.
		sf::Vector2f sizeOffset;

		// Get copy of the Rect that defines this collider.
		inline const Rect GetRect() const { return m_Rect; }

		virtual void Start() override;

		// Takes a Vertex array of size 5, fills it with corners of rect in world-space
		void GetDebugOutline(sf::Vertex* debugOutline) const;

		// Checks for collisions based on a delta time against all other colliders. 
		void CheckForCollisions(float deltaTime);

		// Check for collision against a specific collider using Separating Axis Theorem algorithm. TODO: Add bounding box check before SAT check. 
		void CheckForCollision(std::shared_ptr<CCollider> other, float deltaTime);

		// Calculat moment of inertia (could change if mass, scale, etc changes).
		float GetMomentOfInertia() const;

		// Cache the current transform of the collider before a next frame. 
		void ResetPreviousTransform();
	};
}
