#include "ShapeComponent.h"
#include "ShapeComponent.h"
#include "GameObject.h"
#include "Transform.h"

namespace Engine
{
	ShapeComponent::ShapeComponent()
	{
		m_Shape.setSize(sf::Vector2f(50.0f, 50.0f));
	}

	void ShapeComponent::Update(float deltaTime)
	{
		sf::Vector2f size = m_Shape.getSize();
		sf::Vector2f pivot = m_GameObject->GetTransform()->pivot;
		m_Shape.setOrigin((pivot.x + 0.5f) * size.x, (pivot.y + 0.5f) * size.y);

		m_Shape.setPosition(m_GameObject->GetTransform()->GetPosition());
		m_Shape.setRotation(m_GameObject->GetTransform()->GetRotation());
		m_Shape.setScale(m_GameObject->GetTransform()->GetScale());

		if(spin)
			m_GameObject->GetTransform()->localRotation += (360.f * .25f * deltaTime);
	}
	void ShapeComponent::SetColor(sf::Color color)
	{
		m_Shape.setFillColor(color);
	}
}
