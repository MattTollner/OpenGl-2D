#pragma once
#include <GL\glew.h>


class TextureHelperClass
{
public:

	GLuint ID;
	//Pixels of image
	GLuint Width, Height; 
	
	// Texture Format
	GLuint internalF; 
	GLuint imageF; 

					   
	TextureHelperClass();
	void GenerateTex(GLuint width, GLuint height, unsigned char* data);
	//Binds the texture
	void Bind() const;
};