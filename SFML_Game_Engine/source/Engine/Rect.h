#pragma once

#include "SFML/Graphics.hpp"

namespace Engine
{
	// An Edge, used in Clipping algorithm. 
	struct Edge
	{
		sf::Vector2f maximumProjectionVertex, v0, v1;
		Edge(sf::Vector2f max, sf::Vector2f _v0, sf::Vector2f _v1);
		Edge();
		inline sf::Vector2f GetVector() const { return v1 - v0; }
	};

	// Defines a rectangle in space with size and position. 
	class Rect
	{
	private: 
		float m_Width, m_Height, m_Rotation;
		sf::Vector2f m_Center;
	public:
		Rect();
		Rect(float x, float y, float w, float h);
		inline sf::Vector2f GetSize() const { return sf::Vector2f(m_Width, m_Height); }
		inline sf::Vector2f GetCenter() const { return m_Center; }
		inline float GetRotation() const { return m_Rotation; }
		inline void SetSize(float w, float h) { m_Width = w; m_Height = h; }
		inline void SetSize(const sf::Vector2f& s) { SetSize(s.x, s.y); }
		inline void SetCenter(float x, float y) { m_Center.x = x; m_Center.y = y; }
		inline void SetCenter(const sf::Vector2f& c) { m_Center = c; }
		inline void SetRotation(float r) { m_Rotation = r; }

		// Set all transform data of the rect. 
		inline void Set(const sf::Vector2f& c, float r, const sf::Vector2f& s) { SetCenter(c); SetRotation(r); SetSize(s); }

		// Pass in a Vector2f array of size 4. It will be populated with world-space coordinates of the rect's corners. 
		void  GetCorners(sf::Vector2f*) const;

		// Pass in a Vector2f array of size 2. It will be populated with the 2 unique axes of the rect. 
		void  GetAxes(sf::Vector2f*) const;

		// Get Best Edge of Rect based on a direction vector. Used for Clipping algorithm. 
		Edge GetBestEdge(const sf::Vector2f& dir) const;

	};
}

