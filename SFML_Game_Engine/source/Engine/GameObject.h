#pragma once
#include <memory>
#include <vector>
#include "Updateable.h"
#include "Components/Component.h"
#include <string>

namespace Engine
{
	class GameObject: public Updateable
	{
	private:
		std::vector<std::shared_ptr<Component>> m_Components;
		static std::vector<std::shared_ptr<GameObject>> s_GameObjects;
	public:
		GameObject();
		virtual void Update(float deltaTime) override;
		virtual void Start() override;
		std::shared_ptr<class Transform> m_Transform;
		std::string name;
		static std::vector<std::shared_ptr<GameObject>> GetAllGameObjects();
		static void Instantiate(std::shared_ptr<GameObject> newGO);
		static void Destroy(std::shared_ptr<GameObject> GO);

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
		inline std::shared_ptr<T> GetComponentOfType()
		{
			std::vector<std::shared_ptr<T>> comps = GetComponentsOfType<T>();
			if (comps.size() > 0)
				return comps[0];
			else
				return nullptr;
		};

		template<typename T>
		inline void AddComponent(std::shared_ptr<T> newComponent)
		{
			if (std::shared_ptr<Component> c = std::dynamic_pointer_cast<Component>(newComponent))
			{
				c->SetGameobject(shared_from_base<GameObject>());
				m_Components.push_back(newComponent);
				c->Start();
			}
		};
	};

}