#include "TextHelperClass.h"
#include <iostream>
#include <glm\glm\gtc\matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "ResHelperClass.h"

TextHelperClass::TextHelperClass()
{
}

TextHelperClass::~TextHelperClass()
{
}

//Loads text shaders sets up quads for characers
TextHelperClass::TextHelperClass(GLuint width, GLuint height)
{
	//Loads shaders
	this->TextShader = ResHelperClass::LoadShader("shaders/text.vs", "shaders/text.fs", "text");
	this->TextShader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f), GL_TRUE);
	this->TextShader.SetInt("text", 0);
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextHelperClass::LoadText(std::string font, GLuint fontSize)
{
	// Clear previous characters
	this->Characters.clear();

	FT_Library freeTypeLib;

	//Check for errors
	if (FT_Init_FreeType(&freeTypeLib)) 
		std::cout << "Failed to init free type libary" << std::endl;

	//Loads font face = font
	FT_Face face;
	if (FT_New_Face(freeTypeLib, font.c_str(), 0, &face)){
		std::cout << "Failed to load font" << std::endl;
	}

	//Sets glyph size, 0 lets face dynamically calc witdh based on height (fontSize)
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	//Disable byte-alignment restriction allows for one byte 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Comipiles the first 128 ascii characters from font
	for (GLubyte c = 0; c < 128; c++) 
	{
		//Loads character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Glyph failed to load" << std::endl;
			continue;
		}

		//Create texture for character
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		//
		glTexImage2D(
			GL_TEXTURE_2D, //Target
			0, //Level of detail 0 = base image level
			GL_RED, //internalFormat specifies number of color componets in texture
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED, //format of pixel data
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		//Texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Then stores the character ready for when neeeded
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//Free type no longer needed
	FT_Done_Face(face);
	FT_Done_FreeType(freeTypeLib);
}

//Renders the string of text
void TextHelperClass::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 colour)
{
	//Activates render state	
	this->TextShader.UseShader();
	this->TextShader.SetVector3f("textColor", colour);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	//Loop thorough characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		//Origin position of the quad
		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;
		//Quad size
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos,     ypos,       0.0, 0.0 },

			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 }
		};

		//Render glyph charactrer over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//Shift pixes ready for next quad
		x += (ch.Advance >> 6) * scale; 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
