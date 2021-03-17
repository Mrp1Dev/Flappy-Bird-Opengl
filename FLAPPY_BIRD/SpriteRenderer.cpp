#include <array>
#include "SpriteRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mrp1
{
	void SpriteRenderer::render(glm::mat4 projection) const
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

		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(
			GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		//vertices
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);
		glEnableVertexAttribArray(0);

		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

		glm::mat4 transform { glm::mat4(1.0f) };
		transform = glm::translate(transform, glm::vec3(position.x, position.y, 0.0f));
		transform = glm::scale(transform, glm::vec3(size.x / 2.0f, size.y / 2.0f , 1.0f));
		shader->set_mat4("transform", transform);
		shader->set_mat4("projection", projection);
		shader->set_vec4("color", color);
		shader->use();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}