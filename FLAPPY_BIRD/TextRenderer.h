#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include <iostream>
#include <mrp1\Shader.h>
#include "Sprite.h"
#include FT_FREETYPE_H  

class TextRenderer
{
	struct Character
	{
		unsigned int texture_id;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};
	std::map<char, Character> characters {};
public:
	TextRenderer(std::string font_path)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
			throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
		}

		FT_Face face;
		if (FT_New_Face(ft, font_path.c_str(), 0, &face))
		{
			std::cout << "ERROR::FREETYPE: Failed to load font\n";
			throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
		}

		FT_Set_Pixel_Sizes(face, 0, 256);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (unsigned char c = 0; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph\n";
				continue;
			}

			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			Character character {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};

			characters.insert(std::pair<char, Character>(c, character));
		}
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void render_text(
		Shader* text_shader,
		std::string text,
		glm::vec2 position,
		float scale,
		glm::vec3 color)
	{
		text_shader->set_vec3("text_color", color);
		glActiveTexture(GL_TEXTURE0);
		text_shader->set_int("text", 0);
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = characters[*c];

			float xpos = position.x + ch.bearing.x * scale;
			float ypos = position.y + ch.bearing.y * scale;
			
			float w = ch.size.x * scale;
			float h = ch.size.y * scale;
			xpos += w / 2.0f;
			ypos -= h / 2.0f;
			

			Sprite sprite(glm::vec2(xpos, ypos), glm::vec2(w, h), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			glBindTexture(GL_TEXTURE_2D, ch.texture_id);

			sprite.render(text_shader);
			position.x += (ch.advance >> 6) * scale;
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	float get_text_width(std::string text, float scale = 1.0f)
	{
		float result {};
		
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = characters[*c];

			result += (ch.size.x + ch.bearing.x) * scale;
		}
		return result;
	}
};


