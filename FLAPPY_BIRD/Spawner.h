#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Pillar.h"
#include "random.h"
#include "ScoreTrigger.h"

class ScoreTrigger;

extern const int WIDTH, HEIGHT;
const glm::vec4 PILLAR_COLOR {
	glm::vec4(255.0f / 255.0f, 180.0f / 255.0f, 84.0f / 255.0f, 1.0f)
};
const glm::vec4 SCORE_TRIGGER_COLOR {
	glm::vec4(240.0f / 255.0f, 46.0f / 255.0f, 46.0f / 255.0f, 1.0f)
};
class Spawner
{
	const float PILLAR_WIDTH;
	const float PILLAR_GAP;
	const float PILLAR_SPAWN_DIST;
	const float PILLAR_SPEED;

	Pillar* last_pillar { nullptr };
	std::vector<Pillar> spawned {};
	std::vector<ScoreTrigger> score_triggers {};
public:
	explicit Spawner(
		float pillar_width,
		float pillar_gap,
		float pillar_spawn_dist,
		float pillar_speed
	) : PILLAR_WIDTH { pillar_width },
		PILLAR_GAP { pillar_gap },
		PILLAR_SPAWN_DIST { pillar_spawn_dist },
		PILLAR_SPEED { pillar_speed }
	{}

	void update()
	{
		float last_x = last_pillar != nullptr ? last_pillar->pos.x : 0.0f;
		if ((WIDTH - last_x) > PILLAR_SPAWN_DIST)
		{
			float x_pos = WIDTH + PILLAR_WIDTH / 2.0f;
			float rand_screen_percent = random_range(0.2f, 0.8f);
			for (size_t i = 0; i < 2; i++)
			{
				glm::vec2 pillar_size = glm::vec2(PILLAR_WIDTH, HEIGHT);
				pillar_size.y *=
					i == 0 ? 1.0 - rand_screen_percent : rand_screen_percent;
				float y_pos =
					i == 0 ?
					HEIGHT - (pillar_size.y / 2.0f - PILLAR_GAP / 2.0f)
					: (pillar_size.y / 2.0f - PILLAR_GAP / 2.0f);
				spawned.emplace_back(
					Pillar(glm::vec2(x_pos, y_pos),
						pillar_size,
						PILLAR_COLOR,
						PILLAR_SPEED));
			}
			last_pillar = &spawned.back();

			//SCORE TRIGGERS:
			float y_pos = HEIGHT * rand_screen_percent;
			score_triggers.emplace_back(
				ScoreTrigger(
					glm::vec2(x_pos, y_pos),
					glm::vec2(5.0f, PILLAR_GAP),
					SCORE_TRIGGER_COLOR,
					PILLAR_SPEED
					)
			);
		}

	}

	std::vector<Pillar>* spawned_pillars()
	{
		return &spawned;
	}

	std::vector<ScoreTrigger>* spawned_triggers()
	{
		return &score_triggers;
	}

	void reset()
	{
		spawned.clear();
		last_pillar = nullptr;
		score_triggers.clear();
	}
};
