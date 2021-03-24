#pragma once
#include "Sprite.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Particle: public Sprite
{
	float lifetime {};
	float life { 0.0f };
public:
	glm::vec2 velocity {};
	Particle(glm::vec2 pos, glm::vec2 size, glm::vec4 color, glm::vec2 velocity, float lifetime)
		: Sprite(pos, size, color), velocity { velocity }, lifetime {lifetime}{}
	void update(float dt)
	{
		life += dt;
		if (life > lifetime)
		{
			color.w = 0;
		}
		else
		{
			color.w = 1;
		}
		pos += velocity * dt;
	}

	bool dead()
	{
		return life > lifetime;
	}
};