#pragma once
#include <memory>
#include "SFML/Graphics.hpp"
#include "Updateable.h"

namespace Engine
{
	// Defines Position, Rotation, Scale, and Hierarchy of a GameObject. 
	class Transform : public Updateable
	{
	private:
		// Parent of this. Null if at top level of hierarchy. 
		std::shared_ptr<Transform> m_Parent;

		// Position in relation to Parent. 
		sf::Vector2f localPosition;

		// Rotation in relation to Parent. 
		float localRotation;

		// Scale in relation to Parent. 
		sf::Vector2f localScale;

		// List of all child transforms. 
		std::vector<std::shared_ptr<Transform>> m_Children;

		// Add a child. Only called when a Parent is set. 
		void AddChild(std::shared_ptr<Transform> child);

		// Remove a child. 
		void RemoveChild(std::shared_ptr<Transform> child);

		// Get Rotation value without converting to degrees. 
		float GetRotationRaw() const;

	public:
		Transform();

		virtual void Update(float deltaTime) override;

		// The Pivot defines the "center" of the object. Used to position ShapeComponent. 
		sf::Vector2f pivot;

		// Get the Transformation Matrix representing this Transform in world space. 
		sf::Transform GetMatrix() const;

		// Calculate the World Space position of this Transform. 
		sf::Vector2f GetPosition() const;

		// Calculate the World Space rotation of this Transform. 
		float GetRotation() const;

		// Calculate the World Space scale of this Transform. 
		sf::Vector2f GetScale() const;

		inline sf::Vector2f GetLocalPosition() const { return localPosition; }
		inline float GetLocalRotation() const { return localRotation; }
		inline sf::Vector2f GetLocalScale() const { return localScale; }
		
		inline void SetLocalPosition(const sf::Vector2f& p) { localPosition = p; }
		inline void SetLocalRotation(float r) { localRotation = r; }
		inline void SetLocalScale(const sf::Vector2f& s) { localScale = s; }
		inline void SetPivot(const sf::Vector2f& p) { pivot = p; }

		// Move the object by some amount in local space. 
		void Translate(const sf::Vector2f& t);

		// Rotate the object by some amount in local space. 
		void Rotate(float r);

		// Scale the object by some amount in local space. 
		void Scale(const sf::Vector2f& s);


		inline std::shared_ptr<Transform> GetParent() const  { return m_Parent; }
		inline std::vector<std::shared_ptr<Transform>> GetChildren() const  { return m_Children; }
		void SetParent(std::shared_ptr<Transform> newParent);

	};
}