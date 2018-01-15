#pragma once
#include <map>

#include <GL\glew.h>
#include <glm\glm\glm.hpp>
#include "TextHelperClass.h"
#include "ShaderHelperClass.h"

// Holds character information
struct Character {
	GLuint TextureID;
	//Size of glyph
	glm::ivec2 Size; 
	//Baseline offset to top of glyph
	glm::ivec2 Bearing;
	//Offset of where to put next character
	GLuint Advance;    
};


class TextHelperClass
{
public:
	std::map<GLchar, Character> Characters;
	ShaderHelperClass TextShader;
	TextHelperClass();
	~TextHelperClass();
	TextHelperClass(GLuint width, GLuint height);
	//Loads characters from freetype fnt
	void LoadText(std::string font, GLuint fontSize);
	//Puts everytihbgn together and renders the string
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	GLuint VBO, VAO;
};
