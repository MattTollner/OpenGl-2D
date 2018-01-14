
#include <iostream>

#include "TextureHelperClass.h"


TextureHelperClass::TextureHelperClass()
	: Width(0), Height(0), internalF(GL_RGB), imageF(GL_RGB), wrapS(GL_REPEAT), wrapT(GL_REPEAT), textureMin(GL_LINEAR), textureMax(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

void TextureHelperClass::GenerateTex(GLuint width, GLuint height, unsigned char* data)
{
	this->Width = width;
	this->Height = height;
	// Create Texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internalF, width, height, 0, this->imageF, GL_UNSIGNED_BYTE, data);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->textureMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->textureMax);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureHelperClass::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}