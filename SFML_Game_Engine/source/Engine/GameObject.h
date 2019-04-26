#pragma once
#include <memory>
#include <vector>
#include "Updateable.h"
#include "Components/Component.h"
#include <string>

namespace Engine
{
	// An object which exists in the world of the game, controlled by components.
	class GameObject: public Updateable
	{
	private:
		//List of components attached to this GameObject. 
		std::vector<std::shared_ptr<Component>> m_Components;

		// List of all instantiated GameObjects. 
		static std::vector<std::shared_ptr<GameObject>> s_GameObjects;
	public:
		GameObject();
		virtual void Update(float deltaTime) override;
		virtual void Start() override;

		// Transform defining the GameObjects position, orientation, and scale, as well as position in hierarchy. 
		std::shared_ptr<class Transform> m_Transform;

		// Name of the GameObject
		std::string name;

		// Get list of all GameObjects in the scene. 
		static std::vector<std::shared_ptr<GameObject>> GetAllGameObjects();

		// Instantiate a new GameObject. 
		static void Instantiate(std::shared_ptr<GameObject> newGO);

		// Destroy a GameObject. 
		static void Destroy(std::shared_ptr<GameObject> GO);

		inline std::shared_ptr<class Transform> GetTransform() const { return m_Transform; }
		inline std::vector<std::shared_ptr<Component>> GetComponentsAll() const { return m_Components; }
		
		// Returns all componenets of a specific Type, T. 
		template<typename T>
		inline std::vector<std::shared_ptr<T>> GetComponentsOfType() const
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

		// Returns first component of a specific Type, T. 
		template<typename T>
		inline std::shared_ptr<T> GetComponentOfType() const
		{
			std::vector<std::shared_ptr<T>> comps = GetComponentsOfType<T>();
			if (comps.size() > 0)
				return comps[0];
			else
				return nullptr;
		};

		// Adds a new (already created) Component of Type, T. 
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