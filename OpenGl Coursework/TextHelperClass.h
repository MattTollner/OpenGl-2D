#pragma once
#include <map>

#include <GL\glew.h>
#include <glm\glm\glm.hpp>
#include "TextHelperClass.h"
#include "ShaderHelperClass.h"

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance;     // Horizontal offset to advance to next glyph
};


class TextHelperClass
{
public:
	std::map<GLchar, Character> Characters;
	ShaderHelperClass TextShader;
	TextHelperClass();
	~TextHelperClass();
	TextHelperClass(GLuint width, GLuint height);
	// Compiles characters from font
	void LoadText(std::string font, GLuint fontSize);
	//Creates the string
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	GLuint VBO, VAO;
};
