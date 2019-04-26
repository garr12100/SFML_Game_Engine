#pragma once

#include <memory>
#include "Updateable.h"

namespace Engine
{
	// Components are update-based behavior that can be attached to game objects. 
	class Component : public Updateable
	{
	protected:
		// Reference to owning game object
		std::shared_ptr<class GameObject> m_GameObject;

	public:
		inline std::shared_ptr<class GameObject> GetGameObject() const { return m_GameObject; }
		inline void SetGameobject(std::shared_ptr<class GameObject> gameObject) { m_GameObject = gameObject; }
	};
}