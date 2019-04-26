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


		if(spin)
			m_GameObject->GetTransform()->SetLocalRotation(m_GameObject->GetTransform()->GetLocalRotation() + (360.f * .25f * deltaTime));
		
		if(move)
			m_GameObject->GetTransform()->SetLocalPosition(sf::Vector2f(m_GameObject->GetTransform()->GetLocalPosition().x + (50.f * deltaTime), m_GameObject->GetTransform()->GetLocalPosition().y));
	}
	void ShapeComponent::SetColor(const sf::Color& color)
	{
		m_Shape.setFillColor(color);
	}
	void ShapeComponent::SetSize(const sf::Vector2f& size)
	{
		m_Shape.setSize(size);
	}
}
