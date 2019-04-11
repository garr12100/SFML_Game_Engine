#pragma once
#include <memory>
#include "SFML/Graphics.hpp"
#include "Updateable.h"

namespace Engine
{
	class Transform : public Updateable
	{
	private:
		std::shared_ptr<Transform> m_Parent;
		std::vector<std::shared_ptr<Transform>> m_Children;
		void AddChild(std::shared_ptr<Transform> child);
		void RemoveChild(std::shared_ptr<Transform> child);
	public:
		Transform();
		virtual void Update(float deltaTime) override;
		sf::Vector2f localPosition;
		float localRotation;
		sf::Vector2f localScale;
		sf::Vector2f pivot;

		sf::Transform GetMatrix();
		sf::Vector2f GetPosition();
		float GetRotation();
		sf::Vector2f GetScale();

		inline std::shared_ptr<Transform> GetParent() { return m_Parent; }
		inline std::vector<std::shared_ptr<Transform>> GetChildren() { return m_Children; }
		void SetParent(std::shared_ptr<Transform> newParent);

	};
}