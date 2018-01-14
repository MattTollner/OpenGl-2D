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



// A renderer class for rendering text displayed by a font loaded using the 
// FreeType library. A single font is loaded, processed into a list of Character
// items for later rendering.
class TextHelperClass
{
public:
	// Holds a list of pre-compiled Characters
	std::map<GLchar, Character> Characters;
	// Shader used for text rendering
	ShaderHelperClass TextShader;
	TextHelperClass();
	~TextHelperClass();
	// Constructor
	TextHelperClass(GLuint width, GLuint height);
	// Pre-compiles a list of characters from the given font
	void LoadText(std::string font, GLuint fontSize);
	// Renders a string of text using the precompiled list of characters
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	// Render state
	GLuint VAO, VBO;
};
