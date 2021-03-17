#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <mrp1/Shader.h>

namespace mrp1
{
	class SpriteRenderer
	{
	private:
		Shader* shader;
	public:
		glm::vec2 position;
		glm::vec2 size;
		glm::vec4 color;
		SpriteRenderer(glm::vec2 position, glm::vec2 size, Shader* shader, glm::vec4 color) :
			position { position }, size { size }, shader { shader }, color { color }{}

		void render(glm::mat4 projection) const;
	};
}