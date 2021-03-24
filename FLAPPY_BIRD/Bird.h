#pragma once
#include "Sprite.h"
#include "random.h"
#include <glm/gtc/random.hpp>
#include "particle.h"
#include <vector>

extern const float GRAVITY;
class Bird : public Sprite
{
	glm::vec2 velocity { glm::vec2(0.0f) };
public:
	float jump_strength { 0.0f };
	int score { 0 };
	std::vector<Particle> particles {};
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

		bool collisionX = pos.x < other_pos.x + other->size.x &&
			pos.x + size.x  > other_pos.x;

		bool collisionY = pos.y < other_pos.y + other->size.y &&
			pos.y + size.y > other_pos.y;

		return collisionX && collisionY;
	}

	void burst(
		int particle_count, float min_size, float max_size, float min_speed, float max_speed,
		float min_lifetime, float max_lifetime
	)
	{
		while (particles.size() < particle_count)
		{
			particles.emplace_back(
				Particle(
					glm::vec2(0.0f), glm::vec2(0.0f), glm::vec4(0.0f), glm::vec2(0.0f), 0.0f
				));
		}
		for (size_t i = 0; i < particle_count; i++)
		{
			float rand_size = random_range(min_size,max_size);
			glm::vec2 size(size * rand_size);
			float rand_speed = random_range(min_speed, max_speed);
			glm::vec2 velocity = glm::circularRand(1.0f) * rand_speed;
			float rand_lifetime = random_range(min_lifetime, max_lifetime);
			particles[i] =
				Particle(pos, size, color, velocity, rand_lifetime);
		}
		color.w = 0;
	}
};