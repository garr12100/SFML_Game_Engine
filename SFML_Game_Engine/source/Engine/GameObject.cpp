#include "GameObject.h"
#include "Components/Component.h"
#include "Transform.h"

namespace Engine
{
	GameObject::GameObject()
	{
		m_Transform = std::make_shared<Transform>();
	}

	void GameObject::Update(float deltaTime)
	{
		for (std::shared_ptr<Component> component : m_Components)
		{
			component->Update(deltaTime);
		}
	}


}