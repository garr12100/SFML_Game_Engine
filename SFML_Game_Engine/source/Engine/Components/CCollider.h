#pragma once
#include "Components/Component.h"
#include "Rect.h"
#include <memory>
#include <vector>

namespace Engine
{
	class CCollider : public Component
	{
	private:
		Rect m_Rect;
		std::vector<std::shared_ptr<CCollider>> m_OverlappingWith;
		void HandleCollision(std::shared_ptr<CCollider> other, sf::Vector2f MTV, sf::Vector2f point, float deltaTime);
		void Reposition();
		std::vector<sf::Vector2f> FindContactPoint(std::shared_ptr <CCollider> other, sf::Vector2f dir);
		std::vector<sf::Vector2f> Clip(sf::Vector2f v1, sf::Vector2f v2, sf::Vector2f n, float o);
		sf::Transform m_PreviousTransform;
	public:
		static bool debug;
		bool isTrigger = false;
		std::vector<sf::Vertex> contactVerts;
		std::vector<std::vector<sf::Vertex>> bestEdges;
		sf::Vector2f offset;
		sf::Vector2f sizeOffset;
		inline Rect GetRect() { return m_Rect; }
		virtual void Start() override;
		void GetDebugOutline(sf::Vertex* debugOutline);
		void CheckForCollisions(float deltaTime);
		void CheckForCollision(std::shared_ptr<CCollider> other, float deltaTime);
		float GetMomentOfInertia();
		void ResetPreviousTransform();
	};
}
