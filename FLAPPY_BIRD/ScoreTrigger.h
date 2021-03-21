#pragma once
#include "Pillar.h"

constexpr float FADE_TIME { 0.4f };
class ScoreTrigger : public Pillar
{
public:
	bool scored { false };

	explicit ScoreTrigger(glm::vec2 pos, glm::vec2 size, glm::vec4 color, float speed)
		: Pillar { pos, size, color, speed }
	{
	}

	virtual void update(float delta_time)
	{
		pos.x += -speed * delta_time;
		if (scored)
		{
			color.a -= delta_time / FADE_TIME;
		}
	}
};