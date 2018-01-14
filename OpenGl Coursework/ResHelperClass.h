#pragma once
#include <map>
#include <string>

#include <GL\glew.h>

#include "TextureHelperClass.h"
#include "ShaderHelperClass.h"


//Loads textures and shaders
class ResHelperClass
{
public:
	//Arrays
	static std::map<std::string,ShaderHelperClass>   Shaders;
	static std::map<std::string, TextureHelperClass> Textures;

	static ShaderHelperClass   LoadShader(const GLchar *vertexShader, const GLchar *fragmentShader,  std::string identifier);
	static ShaderHelperClass   GetShader(std::string name); 	//Gets shader from array

	static TextureHelperClass LoadTexture(const GLchar *textureFile, GLboolean alpha, std::string identifier);
	static TextureHelperClass GetTexture(std::string name);
	//Clears loaded resources
	static void      ClearRes();

private:
	ResHelperClass() { }
	static ShaderHelperClass    loadShaderFromFile(const GLchar *vertexShader, const GLchar *fragmentShader);
	static TextureHelperClass loadTextureFromFile(const GLchar *file, GLboolean alpha);
};
