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
		sf::Vector2f localPosition;
		float localRotation;
		sf::Vector2f localScale;
		std::vector<std::shared_ptr<Transform>> m_Children;
		void AddChild(std::shared_ptr<Transform> child);
		void RemoveChild(std::shared_ptr<Transform> child);
		float GetRotationRaw();

	public:
		Transform();
		virtual void Update(float deltaTime) override;

		sf::Vector2f pivot;

		sf::Transform GetMatrix();

		sf::Vector2f GetPosition();
		float GetRotation();
		sf::Vector2f GetScale();

		inline sf::Vector2f GetLocalPosition() { return localPosition; }
		inline float GetLocalRotation() { return localRotation; }
		inline sf::Vector2f GetLocalScale() { return localScale; }
		
		inline void SetLocalPosition(sf::Vector2f p) { localPosition = p; }
		inline void SetLocalRotation(float r) { localRotation = r; }
		inline void SetLocalScale(sf::Vector2f s) { localScale = s; }

		void Translate(sf::Vector2f t);
		void Rotate(float r);
		void Scale(sf::Vector2f s);

		inline std::shared_ptr<Transform> GetParent() { return m_Parent; }
		inline std::vector<std::shared_ptr<Transform>> GetChildren() { return m_Children; }
		void SetParent(std::shared_ptr<Transform> newParent);

	};
}