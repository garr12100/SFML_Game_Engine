#pragma once
#include <memory>
#include <vector>
#include "Updateable.h"
#include "Components/Component.h"

namespace Engine
{
	class GameObject: public Updateable
	{
	private:
		std::vector<std::shared_ptr<Component>> m_Components;
	public:
		std::shared_ptr<class Transform> m_Transform;

		GameObject();
		virtual void Update(float deltaTime) override;
		inline std::shared_ptr<class Transform> GetTransform() { return m_Transform; }
		inline std::vector<std::shared_ptr<Component>> GetComponentsAll() { return m_Components; }
		
		template<typename T>
		inline std::vector<std::shared_ptr<T>> GetComponentsOfType()
		{
			std::vector<std::shared_ptr<T>> comps;
			
			for (std::shared_ptr<Component> component : m_Components)
			{
				if (std::shared_ptr<T> foundComp = std::dynamic_pointer_cast<T>(component))
				{
					comps.push_back(foundComp);
				}
			}
			return comps;
		};

		template<typename T>
		inline void AddComponent(std::shared_ptr<T> newComponent)
		{
			if (std::shared_ptr<Component> c = std::dynamic_pointer_cast<Component>(newComponent))
			{
				c->SetGameobject(shared_from_base<GameObject>());
				m_Components.push_back(newComponent);
			}
		};
	};

}