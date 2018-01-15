
#include <iostream>
#include "TextureHelperClass.h"


TextureHelperClass::TextureHelperClass() : Width(0), Height(0), imageF(GL_RGB), internalF(GL_RGB)
{
	glGenTextures(1, &this->ID);
}

//Creates texture and manages its format/filters/wrapping 
void TextureHelperClass::GenerateTex(GLuint width, GLuint height, unsigned char* data)
{
	this->Width = width;
	this->Height = height;
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internalF, width, height, 0, this->imageF, GL_UNSIGNED_BYTE, data);
	//Manages texture wrapping and filters 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0); //unbinds texture
}

//Binds texture
void TextureHelperClass::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}