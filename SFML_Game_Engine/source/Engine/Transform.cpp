#include "Transform.h"

namespace Engine
{
	Transform::Transform()
		: localPosition(0.f, 0.f), localRotation(0.f), localScale(1.f, 1.f), pivot(0.f, 0.f)
	{
	}

	void Transform::Update(float deltaTime)
	{
		while (localRotation > 360)
			localRotation = localRotation - 360;
		while (localRotation < 0)
			localRotation = localRotation + 360;

		
	}

	sf::Transform Transform::GetMatrix()
	{
		sf::Transform matrix;
		matrix.translate(localPosition);
		matrix.rotate(localRotation);
		matrix.scale(localScale);
		return matrix;
	}

	sf::Vector2f Transform::GetPosition()
	{
		sf::Vector2f position = localPosition;
		if (m_Parent)
		{
			position += m_Parent->GetPosition();
		}

		return position;
	}

	float Transform::GetRotation()
	{
		float rotation = localRotation;
		if (m_Parent)
		{
			rotation += m_Parent->GetRotation();
			while (rotation > 360)
				rotation = rotation - 360;
			while (rotation < 0)
				rotation = rotation + 360;
		}
		return rotation;
	}

	sf::Vector2f Transform::GetScale()
	{
		sf::Vector2f scale = localScale;
		if (m_Parent)
		{
			scale.x *= m_Parent->GetScale().x;
			scale.y *= m_Parent->GetScale().y;
		}
		return scale;
	}

	void Transform::AddChild(std::shared_ptr<Transform> child)
	{
		m_Children.push_back(child);
	}

	void Transform::RemoveChild(std::shared_ptr<Transform> child)
	{
		auto index = std::find(m_Children.begin(), m_Children.end(), child);
		if (index != m_Children.end())
			m_Children.erase(index);
	}

	void Transform::SetParent(std::shared_ptr<Transform> newParent)
	{
		if (m_Parent)
		{
			m_Parent->RemoveChild(shared_from_base<Transform>());
		}
		m_Parent = newParent;
		newParent->AddChild(shared_from_base<Transform>());
	}
}
