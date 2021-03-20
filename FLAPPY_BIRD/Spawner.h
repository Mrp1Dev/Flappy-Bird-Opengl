#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Pillar.h"
#include "random.h"

extern const int WIDTH, HEIGHT;
class Spawner
{
	const float PILLAR_WIDTH;
	const float PILLAR_GAP;
	const float PILLAR_SPAWN_DIST;
	const float PILLAR_SPEED;

	Pillar* last_pillar { nullptr };
	std::vector<Pillar> spawned {};
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
						glm::vec4(0.9f, 0.2f, 0.2f, 1.0f),
						PILLAR_SPEED));
				std::cout << "pillar spawn for loop hello\n";
			}
			last_pillar = &spawned.back();
		}
	}

	std::vector<Pillar>* spawned_pillars()
	{
		return &spawned;
	}

	void reset()
	{
		spawned.clear();
		last_pillar = nullptr;
	}
};
