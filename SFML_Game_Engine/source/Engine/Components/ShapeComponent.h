#pragma once

#include "Component.h"
#include "SFML/Graphics.hpp"

namespace Engine
{
	// Simple rectangle shape component. TODO: Create a Renderable class and derive various render shapes and sprites from it. 
	class ShapeComponent : public Component
	{
	private: 
		//SFML Rectangle Shape that defines this object
		sf::RectangleShape m_Shape;

	public:
		ShapeComponent();
		virtual void Update(float deltaTime) override;
		inline sf::RectangleShape GetShape() const { return m_Shape; }

		// Set the color of the rectangle 
		void SetColor(const sf::Color& color);

		//Set the size of the rectangle
		void SetSize(const sf::Vector2f& size);

		//Spin the rectangle. Used for debugging. 
		bool spin = false;

		//Make the rectangle move. Used for debugging. 
		bool move = false;

	
	};
}