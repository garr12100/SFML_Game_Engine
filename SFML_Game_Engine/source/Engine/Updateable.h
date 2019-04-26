#pragma once

namespace Engine
{
	// Base class for objects that need Start and Update functions. 
	class Updateable : public std::enable_shared_from_this<Updateable>
	{
	protected:
		// Get 'this' pointer if there is a SharedPointer pointing to this object. 
		template <typename Derived>
		std::shared_ptr<Derived> shared_from_base()
		{
			return std::static_pointer_cast<Derived>(shared_from_this());
		}
	public:
		// Called Every Frame. 
		virtual void Update(float deltaTime) {}

		// Called when object is created. 
		virtual void Start() {}
	};
}