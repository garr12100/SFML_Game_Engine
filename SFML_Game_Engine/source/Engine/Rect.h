#pragma once

#include "SFML/Graphics.hpp"

namespace Engine
{
	struct Edge
	{
		sf::Vector2f maximumProjectionVertex, v0, v1;
		Edge(sf::Vector2f max, sf::Vector2f _v0, sf::Vector2f _v1);
		Edge();
		inline sf::Vector2f GetVector() { return v1 - v0; }
	};

	class Rect
	{
	private: 
		float m_Width, m_Height, m_Rotation;
		sf::Vector2f m_Center;
	public:
		Rect();
		Rect(float x, float y, float w, float h);
		inline sf::Vector2f GetSize() { return sf::Vector2f(m_Width, m_Height); }
		inline sf::Vector2f GetCenter() { return m_Center; }
		inline float GetRotation() { return m_Rotation; }
		inline void SetSize(float w, float h) { m_Width = w; m_Height = h; }
		inline void SetSize(sf::Vector2f s) { SetSize(s.x, s.y); }
		inline void SetCenter(float x, float y) { m_Center.x = x; m_Center.y = y; }
		inline void SetCenter(sf::Vector2f c) { m_Center = c; }
		inline void SetRotation(float r) { m_Rotation = r; }
		inline void Set(sf::Vector2f c, float r, sf::Vector2f s) { SetCenter(c); SetRotation(r); SetSize(s); }
		void GetCorners(sf::Vector2f*);
		void GetAxes(sf::Vector2f*);
		Edge GetBestEdge(sf::Vector2f dir);

	};
}

