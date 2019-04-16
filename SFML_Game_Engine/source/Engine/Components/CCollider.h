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
	public:
		static bool debug;
		bool isTrigger = false;
		sf::Vector2f offset;
		sf::Vector2f sizeOffset;
		inline Rect GetRect() { return m_Rect; }
		virtual void Start() override;
		void GetDebugOutline(sf::Vertex* debugOutline);
		void CheckForCollisions(float deltaTime);
		void CheckForCollision(std::shared_ptr<CCollider> other, float deltaTime);
		float GetMomentOfInertia();
	};
}
