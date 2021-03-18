#pragma once
#include <vector>
#include <memory>

namespace mrp1
{
	class Component;
	class GameObject
	{
	public:
		GameObject()
		{
		}

		std::vector<std::unique_ptr<Component>> components;
		void update_components();
	};
}