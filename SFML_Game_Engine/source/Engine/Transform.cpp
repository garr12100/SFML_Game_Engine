#include "Transform.h"
#include <math.h> 

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

	sf::Transform Transform::GetMatrix() const
	{
		sf::Transform matrix;
		matrix.translate(localPosition).rotate(localRotation).scale(localScale);
		return m_Parent ? m_Parent->GetMatrix() * matrix : matrix;
	}

	sf::Vector2f Transform::GetPosition() const
	{
		const float* matrix = GetMatrix().getMatrix();

		return sf::Vector2f(matrix[12], matrix[13]);
	}

	float Transform::GetRotation() const
	{
		const float* matrix = GetMatrix().getMatrix();
		float r = std::atan2f(matrix[1], matrix[0]) * 180.f / 3.14159;
		return r;
	}

	float Transform::GetRotationRaw() const
	{
		const float* matrix = GetMatrix().getMatrix();
		float r = std::atan2f(matrix[1], matrix[0]);
		return r;
	}

	sf::Vector2f Transform::GetScale() const
	{
		const float* matrix = GetMatrix().getMatrix();
		float x = matrix[0] / std::cos(GetRotationRaw());
		float y = matrix[5] / std::cos(GetRotationRaw());
		return sf::Vector2f(x, y);
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

	void Transform::Translate(const sf::Vector2f& t)
	{
		localPosition += t;
	}

	void Transform::Rotate(float r)
	{
		localRotation += r;
	}

	void Transform::Scale(const sf::Vector2f& s)
	{
		localScale.x *= s.x;
		localScale.y *= s.y;
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
