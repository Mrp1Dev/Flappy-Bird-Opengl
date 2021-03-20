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
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Bird* bird);
void init_opengl();

static const float GRAVITY { 1000.0f };
static const int WIDTH { 1344 };
static const int HEIGHT { 756 };
constexpr float CLEAR_COLOR[4] { 10.0f / 255.0f, 14.0f / 255.0f, 20.0f / 255.0f, 1.0f };
const glm::vec4 BIRD_COLOR { 89.0f / 255.0f, 194.0f / 255.0f, 255.0f / 255.0f, 1.0f };

int main()
{
	try
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "First Project", nullptr, nullptr);
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

		Shader shader = Shader("vertex_shader.glsl", "fragment_shader.glsl");

		glm::mat4 projection { glm::ortho(
			0.0f, (float)WIDTH, 0.0f, (float)HEIGHT , -1.0f, 1.0f) };


		std::unique_ptr<Bird> bird = std::make_unique<Bird>(
			glm::vec2(WIDTH / 4, HEIGHT / 2),
			glm::vec2(30, 30),
			BIRD_COLOR,
			75.0f
			);
		std::unique_ptr<Spawner> spawner = std::make_unique<Spawner>(
			60.0f,
			155.0f,
			380.0f,
			350.0f
			);
		float last_frame_time = glfwGetTime();
		float last_pillar_pos { 0.0f };

		while (!glfwWindowShouldClose(window))
		{
			float delta_time = glfwGetTime() - last_frame_time;
			last_frame_time = glfwGetTime();
			std::cout << roundf(1.0f / delta_time) << '\n';
			processInput(window, bird.get());

			glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
			glClear(GL_COLOR_BUFFER_BIT);
			shader.set_mat4("projection", projection);

			bird->update(delta_time);
			spawner->update();
			for (auto& pillar : *spawner->spawned_pillars())
			{
				pillar.update(delta_time);
				pillar.render(&shader);
			}
			bird->render(&shader);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
	}
	catch (std::exception e)
	{
		std::cout << "ERROR: " << e.what() << '\n';
	}
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
		bird->flap();
	}
}

void init_opengl()
{
	float vertices[] {
		//positions			
		1.0f, 1.0f, 0.0f,   //top right
		1.0f, -1.0f, 0.0f,  //bottom right
		-1.0f, -1.0f, 0.0f, //bottom left
		-1.0f, 1.0f, 0.0f,  //top left
	};

	unsigned int indices[] {
		0, 1, 3, //top triangle
		1, 2, 3	 //bottom triangle
	};

	unsigned int EBO, VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	//vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

}



