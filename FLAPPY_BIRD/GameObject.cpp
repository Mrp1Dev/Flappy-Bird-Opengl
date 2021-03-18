#include "GameObject.h"
#include "Component.h"

namespace mrp1
{
	void GameObject::update_components()
	{
		for (auto& component : components)
		{
			component->update(this);
		}
	}
}