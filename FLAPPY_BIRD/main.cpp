#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SpriteRenderer.h"

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


	//float vertices[] {
	//	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //bootm left
	//	0.0f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,//top
	//	0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,//bottom right
	//};

	//unsigned int VAO;
	//glGenVertexArrays(1, &VAO);
	//unsigned int VBO;
	//glGenBuffers(1, &VBO);


	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(
	//	GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	////vertices
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, nullptr);
	//glEnableVertexAttribArray(0);
	////color attribs
	//glVertexAttribPointer(
	//	1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
	//glEnableVertexAttribArray(1);


	Shader shader = Shader("vertex_shader.glsl", "fragment_shader.glsl");
	mrp1::SpriteRenderer renderer(
		glm::vec2(400, 300),
		glm::vec2(BASE_WIDTH
			, 100),
		&shader,
		glm::vec4(1, 1, 1, 1));
	glm::mat4 projection { glm::ortho(
		0.0f, (float)BASE_WIDTH, 0.0f, (float)BASE_HEIGHT , -1.0f, 1.0f) };
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], CLEAR_COLOR[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		renderer.render(projection);
		
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
