#include "ResHelperClass.h"
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <Simple OpenGL Image Library\src\SOIL.h>

// Instantiate static variables
std::map<std::string, TextureHelperClass>    ResHelperClass::Textures;
std::map<std::string, ShaderHelperClass>       ResHelperClass::Shaders;



ShaderHelperClass ResHelperClass::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
	return Shaders[name];
}

ShaderHelperClass ResHelperClass::GetShader(std::string name)
{
	return Shaders[name];
}

TextureHelperClass ResHelperClass::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

TextureHelperClass ResHelperClass::GetTexture(std::string name)
{
	return Textures[name];
}

void ResHelperClass::Clear()
{
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	// (Properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

ShaderHelperClass ResHelperClass::loadShaderFromFile(const GLchar *vertShader, const GLchar *fragShader)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vertShader);
		std::ifstream fragmentShaderFile(fragShader);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		
	}
	catch (std::exception e)
	{
		std::cout << "Failed loading shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	// 2. Now create shader object from source code
	ShaderHelperClass shader;
	shader.Compile(vShaderCode, fShaderCode);
	return shader;
}

TextureHelperClass ResHelperClass::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
	// Create Texture object
	TextureHelperClass texture;
	if (alpha)
	{
		texture.internalF = GL_RGBA;
		texture.imageF = GL_RGBA;
	}
	// Load image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.imageF == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Now generate texture
	texture.GenerateTex(width, height, image);
	// And finally free image data
	SOIL_free_image_data(image);
	return texture;
}