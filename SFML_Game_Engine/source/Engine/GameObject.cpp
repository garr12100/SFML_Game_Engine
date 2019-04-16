#include "GameObject.h"
#include "Components/Component.h"
#include "Transform.h"

namespace Engine
{

	std::vector<std::shared_ptr<GameObject>> GameObject::s_GameObjects;

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

	void GameObject::Start()
	{
		for (std::shared_ptr<Component> component : m_Components)
		{
			component->Start();
		}
	}

	std::vector<std::shared_ptr<GameObject>> GameObject::GetAllGameObjects()
	{
		return s_GameObjects; 
	}

	void GameObject::Instantiate(std::shared_ptr<GameObject> newGO)
	{
		s_GameObjects.push_back(newGO);
		newGO->Start();
	}

	void GameObject::Destroy(std::shared_ptr<GameObject> GO)
	{
		auto index = std::find(s_GameObjects.begin(), s_GameObjects.end(), GO);
		if (index != s_GameObjects.end())
			s_GameObjects.erase(index);
	}


}