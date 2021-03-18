#pragma once
#include "Sprite.h"

class Pillar : public Sprite
{
public:
	float speed { 0.0f };
	explicit Pillar(glm::vec2 pos, glm::vec2 size, glm::vec4 color, float speed)
		: Sprite { pos, size, color }, speed { speed }{}

	void update(float dt)
	{
		pos.x += -speed * dt;
	}
};