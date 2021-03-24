#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mrp1/Shader.h>
#include "Bird.h"
#include "Pillar.h"
#include <vector>
#include "Spawner.h"
#include "ScoreTrigger.h"
#include "TextRenderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Bird* bird);
void init_opengl();
void restart_game(Spawner* spawner, Bird* bird);

static const float GRAVITY { 1000.0f };
static const int WIDTH { 1920 };
static const int HEIGHT { 1000 };
constexpr float CLEAR_COLOR[4] { 10.0f / 255.0f, 14.0f / 255.0f, 20.0f / 255.0f, 1.0f };
const glm::vec4 BIRD_COLOR { 89.0f / 255.0f, 194.0f / 255.0f, 255.0f / 255.0f, 1.0f };
const glm::vec4 SCORE_COLOR { 149 / 255.0f, 230 / 255.0f, 203 / 255.0f , 1.0f };
const Bird STARTING_BIRD(
	glm::vec2(WIDTH / 4, HEIGHT / 2),
	glm::vec2(30, 30),
	BIRD_COLOR,
	75.0f);

enum class GameState
{
	Started,
	Running,
	Ended
};

GameState game_state { GameState::Started };

int main()
{
	srand(glfwGetTime());
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Flappy Bird", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW Window.\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}
	glViewport(0, 0, WIDTH, HEIGHT);

	init_opengl();

	Shader text_shader = Shader("text_vertex_shader.glsl", "text_fragment_shader.glsl");
	Shader shader = Shader("vertex_shader.glsl", "fragment_shader.glsl");

	glm::mat4 projection { glm::ortho(
		0.0f, (float)WIDTH, 0.0f, (float)HEIGHT , -1.0f, 1.0f) };


	std::unique_ptr<Bird> bird = std::make_unique<Bird>(
		STARTING_BIRD
		);
	std::unique_ptr<Spawner> spawner = std::make_unique<Spawner>(
		60.0f,
		165.0f,
		385.0f,
		350.0f
		);
	float last_frame_time = glfwGetTime();
	float last_pillar_pos { 0.0f };
	std::unique_ptr<TextRenderer> score_renderer {
		std::make_unique<TextRenderer>("Roboto-Thin.ttf")
	};

	std::unique_ptr<TextRenderer> highscore_renderer {
		std::make_unique<TextRenderer>("Roboto-Black.ttf")
	};

	int highscore { 0 };
	spawner->spawn_borders();
	while (!glfwWindowShouldClose(window))
	{
		float delta_time = glfwGetTime() - last_frame_time;
		last_frame_time = glfwGetTime();
		//std::cout << roundf(1.0f / delta_time) << '\n';
		processInput(window, bird.get());
		glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		shader.set_mat4("projection", projection);
		if (game_state == GameState::Running)
		{
			bird->update(delta_time);
			spawner->update();
			for (auto& pillar : *spawner->spawned_pillars())
			{
				pillar.update(delta_time);
				bool collided = bird->check_collision(&pillar);
				if (collided)
				{
					game_state = GameState::Ended;
					bird->burst(
						64,
						0.05f,
						0.45f,
						600.0f,
						1250.0f,
						0.1f,
						0.8f
					);
				}
			}

			for (auto& trigger : *spawner->spawned_triggers())
			{
				trigger.update(delta_time);
				if (!trigger.scored)
				{
					bool collided = bird->check_collision(&trigger);
					if (collided)
					{
						trigger.scored = true;
						std::cout << bird->score++ << '\n';
					}
				}
			}
		}
		if (game_state == GameState::Ended)
		{
			bool should_restart { true };
			for (auto& particle : bird->particles)
			{
				if (!particle.dead())
				{
					should_restart = false;
					break;
				}
			}
			if (should_restart) restart_game(spawner.get(), bird.get());
		}

		for (auto& particle : bird->particles)
		{
			particle.update(delta_time);
			particle.render(&shader);
		}

		if (highscore < bird->score) highscore = bird->score;

		bird->render(&shader);
		for (auto& pillar : *spawner->spawned_pillars())
		{
			pillar.render(&shader);
		}

		for (auto& trigger : *spawner->spawned_triggers())
		{
			trigger.render(&shader);
		}

		text_shader.use();
		constexpr float TEXT_SCALE = 0.35f;
		text_shader.set_mat4("projection", projection);
		glm::vec2 center_aligned_pos(
			(WIDTH - score_renderer->get_text_width(std::to_string(bird->score), TEXT_SCALE)) / 2.0f,
			HEIGHT - 120
		);
		score_renderer->render_text(
			&text_shader,
			std::to_string(bird->score),
			center_aligned_pos,
			TEXT_SCALE,
			SCORE_COLOR
		);
		center_aligned_pos = glm::vec2(
			(WIDTH - score_renderer->get_text_width(std::to_string(highscore), TEXT_SCALE * 0.4f)) / 2.0f,
			HEIGHT - 165
		);
		highscore_renderer->render_text(
			&text_shader,
			std::to_string(highscore),
			center_aligned_pos,
			TEXT_SCALE * 0.4f,
			SCORE_COLOR
		);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Bird* bird)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (game_state == GameState::Started)
		{
			game_state = GameState::Running;
		}
		else if (game_state == GameState::Running)
		{
			bird->flap();
		}
	}
}

void init_opengl()
{
	float vertices[] {
		//positions			

		-1.0f, 1.0f,   0.0f, 1.0f, //top left
		-1.0f, -1.0f,  0.0f, 0.0f, //bottom left
		1.0f, -1.0f,   1.0f, 0.0f,  //bottom right

		-1.0f, 1.0f,   0.0f, 1.0f, //top left
		1.0f, 1.0f,    1.0f, 1.0f,  //top right
		1.0f, -1.0f,   1.0f, 0.0f,  //bottom right
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	//vertices
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
	/*glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);*/
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void restart_game(Spawner* spawner, Bird* bird)
{
	game_state = GameState::Started;
	spawner->reset();
	auto particles = std::move(bird->particles);
	*bird = STARTING_BIRD;
	bird->particles = std::move(particles);
}




