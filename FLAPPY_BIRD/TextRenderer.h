#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
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
	TextRenderer(std::string font_path, )
};