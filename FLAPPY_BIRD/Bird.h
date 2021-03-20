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

	bool check_collision(Sprite* other)
	{
		glm::vec2 pos = glm::vec2(
			this->pos.x - size.x / 2.0f,
			this->pos.y - size.y / 2.0f);
		glm::vec2 other_pos = glm::vec2(
			other->pos.x - other->size.x / 2.0f,
			other->pos.y - other->size.y / 2.0f);

		bool collisionX = pos.x < other_pos.x + other->size.x  &&
			pos.x + size.x  > other_pos.x;
		std::cout << "Collision on X!\n";
		bool collisionY = pos.y < other_pos.y + other->size.y &&
			pos.y + size.y > other_pos.y;
		
		return collisionX && collisionY;
	}
};