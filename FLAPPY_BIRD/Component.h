#pragma once

namespace mrp1
{
	class GameObject;
	class Component
	{
	public:
		Component()
		{
		}
		virtual void update(GameObject* game_object) = 0;
	};
}