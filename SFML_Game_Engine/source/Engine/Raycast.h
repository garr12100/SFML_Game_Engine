#pragma once
#include "SFML/Graphics.hpp"
#include <memory>

namespace Engine
{
	// Contains data about a Raycast Hit. 
	struct RaycastHit
	{
		std::shared_ptr<class CCollider> collider;
		sf::Vector2f point;
		sf::Vector2f normal;
	};

	// Line Segment
	struct LineSegment
	{
		sf::Vector2f start;
		sf::Vector2f end;
		LineSegment(sf::Vector2f s, sf::Vector2f e);
	};

	// Used to perform a Raycast and to debug Raycast data. 
	class Raycast
	{
	private:
		// List of all current Raycasts being performed. Used for debugging. 
		static std::vector<std::vector<sf::Vertex>> m_CurrentRaycasts;

	public:
		// Display current Raycast in the game window? 
		static bool debug;

		// Performs a raycast. 
		// Takes a start position, direction, and length, as well as references to a RaycastHit and a Collider to ignore (typically the collider owned by the calling GameObject)
		//
		// Returns True if the Raycast hits an object, and will then populate 'hit' with data about the closest collision. 
		static bool PerformRaycast(const sf::Vector2f& startPosition, sf::Vector2f direction, float length, RaycastHit & hit, class CCollider* ignore = nullptr, bool debuggable = true);
		
		static std::vector<std::vector<sf::Vertex>> GetCurrentRaycasts();

		//Clear all current Raycasts from the list. 
		static void ClearCurrentRaycasts();
	};
}