#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <mrp1/Shader.h>
#include <vector>
#include "SpriteRenderer.h"

namespace mrp1
{
	class GameRenderer
	{
		glm::mat4 projection;
		Shader* shader;
		unsigned int VBO, VAO, EBO;
	public:
		GameRenderer(glm::mat4 projection, Shader* shader);
		~GameRenderer();
		std::vector<SpriteRenderer*> sprites;
		void render();
	};
}
