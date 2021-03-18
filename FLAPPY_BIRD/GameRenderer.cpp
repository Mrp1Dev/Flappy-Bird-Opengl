#include "GameRenderer.h"

namespace mrp1
{
	GameRenderer::GameRenderer(glm::mat4 projection, Shader* shader) 
		: projection { projection }, shader { shader }
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

	GameRenderer::~GameRenderer()
	{
		glDisableVertexAttribArray(0);
		glDeleteBuffers(1, &EBO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &VAO);
	}

	void GameRenderer::render()
	{
		for (auto& sprite : sprites)
		{
			shader->set_mat4("transform",sprite->get_transform_matrix());
			shader->set_mat4("projection", projection);
			shader->set_vec4("color", sprite->color);
			shader->use();

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}
}