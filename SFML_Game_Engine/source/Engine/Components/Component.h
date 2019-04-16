#pragma once

#include <memory>
#include "Updateable.h"

namespace Engine
{
	class Component : public Updateable
	{
	protected:
		std::shared_ptr<class GameObject> m_GameObject;

	public:
		inline std::shared_ptr<class GameObject> GetGameObject() { return m_GameObject; }
		inline void SetGameobject(std::shared_ptr<class GameObject> gameObject) { m_GameObject = gameObject; }
	};
}