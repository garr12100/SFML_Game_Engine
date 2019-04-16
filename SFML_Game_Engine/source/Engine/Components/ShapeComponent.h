#pragma once

#include "Component.h"
#include "SFML/Graphics.hpp"

namespace Engine
{
	class ShapeComponent : public Component
	{
	private: 
		sf::RectangleShape m_Shape;

	public:
		ShapeComponent();
		virtual void Update(float deltaTime) override;
		inline sf::RectangleShape GetShape() { return m_Shape; }
		void SetColor(sf::Color color);
		bool spin = false;
		bool move = false;

	
	};
}