#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <mrp1/Shader.h>
#include "Component.h"
namespace mrp1
{
	class SpriteRenderer : public Component
	{
	private:
		unsigned int EBO;
		unsigned int VAO;
		unsigned int VBO;
		Shader* shader;
		const glm::mat4 projection;
	public:
		glm::vec2 position;
		glm::vec2 size;
		glm::vec4 color;
		SpriteRenderer(
			glm::mat4 projection,
			glm::vec2 position,
			glm::vec2 size,
			Shader* shader,
			glm::vec4 color);
		~SpriteRenderer();
		void update(GameObject* game_object);
		glm::mat4 get_transform_matrix();
	};
}