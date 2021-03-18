#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <mrp1/Shader.h>

class Sprite
{
public:
	glm::vec2 pos;
	glm::vec2 size;
	glm::vec4 color;
	Sprite(glm::vec2 pos, glm::vec2 size, glm::vec4 color)
		: pos { pos }, size { size }, color { color }{}

	void render(Shader* shader)
	{
		glm::mat4 transform { glm::mat4(1.0f) };
		transform = glm::translate(transform, glm::vec3(pos.x, pos.y, 0.0f));
		transform = glm::scale(transform, glm::vec3(size.x / 2.0f, size.y / 2.0f, 1.0f));
		shader->set_mat4("transform", transform);
		shader->set_vec4("color", color);
		shader->use();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};