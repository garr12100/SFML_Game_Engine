#pragma once

namespace Engine
{
	class Updateable : public std::enable_shared_from_this<Updateable>
	{
	protected:
		template <typename Derived>
		std::shared_ptr<Derived> shared_from_base()
		{
			return std::static_pointer_cast<Derived>(shared_from_this());
		}
	public:
		virtual void Update(float deltaTime) = 0;
	};
}