#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Component.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

constexpr int BASE_WIDTH { 800 };
constexpr int BASE_HEIGHT { 600 };
constexpr float CLEAR_COLOR[4] { 10.0f / 255.0f, 14.0f / 255.0f, 20.0f / 255.0f, 1.0f };
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "First Project", nullptr, nullptr);
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
	glViewport(0, 0, BASE_WIDTH, BASE_HEIGHT);

	Shader shader = Shader("vertex_shader.glsl", "fragment_shader.glsl");

	glm::mat4 projection { glm::ortho(
		0.0f, (float)BASE_WIDTH, 0.0f, (float)BASE_HEIGHT , -1.0f, 1.0f) };

	mrp1::GameObject bird {};

	mrp1::SpriteRenderer spriteRenderer(
		projection,
		glm::vec2(200, BASE_HEIGHT / 2),
		glm::vec2(30, 30),
		&shader,
		glm::vec4(0.2f, 0.7f, 1.0f, 1.0f));

	bird.components.emplace_back(
		std::make_unique<mrp1::SpriteRenderer>(spriteRenderer));

	mrp1::GameObject bird_2 {};

	mrp1::SpriteRenderer spriteRenderer_2(
		projection,
		glm::vec2(350, BASE_HEIGHT / 2),
		glm::vec2(30, 30),
		&shader,
		glm::vec4(0.2f, 0.7f, 1.0f, 1.0f));

	bird_2.components.emplace_back(
		std::make_unique<mrp1::SpriteRenderer>(spriteRenderer_2));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		try
		{
			bird.update_components();
			bird_2.update_components();
		}
		catch (std::exception e)
		{
			std::cout << e.what() << '\n';
		}
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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
