#pragma once
#include "Sprite.h"

extern const float GRAVITY;
class Bird : public Sprite
{
	glm::vec2 velocity { glm::vec2(0.0f) };
public:
	float jump_strength { 0.0f };
	explicit Bird(glm::vec2 pos, glm::vec2 size, glm::vec4 color, float flap_height)
		: Sprite { pos, size, color }, jump_strength { flap_height }{}
	void update(float dt)
	{
		velocity.y -= GRAVITY * dt;
		pos += velocity * dt;

	}

	void flap()
	{
		velocity.y = sqrt(2.0f * jump_strength * GRAVITY);
	}
};