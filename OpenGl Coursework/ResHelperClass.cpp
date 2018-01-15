#include "ResHelperClass.h"
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <Simple OpenGL Image Library\src\SOIL.h>

//Stores textures/shaders data
std::map<std::string, TextureHelperClass>    ResHelperClass::TexturesList;
std::map<std::string, ShaderHelperClass>       ResHelperClass::ShadersList;


//Calls the parse shader function and adds result to ShaderList
ShaderHelperClass ResHelperClass::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name)
{
	ShadersList[name] = parseShaderFiles(vShaderFile, fShaderFile);
	return ShadersList[name];
}
//Gets shader by name from list
ShaderHelperClass ResHelperClass::GetShader(std::string name)
{
	return ShadersList[name];
}
//Calls the parse texture function and adds result to TextureList
TextureHelperClass ResHelperClass::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	TexturesList[name] = parseTextureFiles(file, alpha);
	return TexturesList[name];
}
//Gets texture by name from list
TextureHelperClass ResHelperClass::GetTexture(std::string name)
{
	return TexturesList[name];
}

//Cleanup shaders/textures stored
void ResHelperClass::ClearRes()
{

	for (auto shader : ShadersList)
	{
		glDeleteProgram(shader.second.ID);
	}
	
	for (auto texture : TexturesList)
	{
		glDeleteTextures(1, &texture.second.ID);
	}
		
}

//Private Functions--------

//Reads shader files
ShaderHelperClass ResHelperClass::parseShaderFiles(const GLchar *vertShader, const GLchar *fragShader)
{
	//Data from file
	std::string vertexContent;
	std::string fragmentContent;
	try
	{
		std::ifstream vertexShaderFile(vertShader);
		std::ifstream fragmentShaderFile(fragShader);
		std::stringstream vSS, fSS;
		
		//buffer into string stream
		vSS << vertexShaderFile.rdbuf();
		fSS << fragmentShaderFile.rdbuf();
		//Closes file
		vertexShaderFile.close();
		fragmentShaderFile.close();
		
		vertexContent = vSS.str();
		fragmentContent = fSS.str();
		
	}
	catch (std::exception e)
	{
		std::cout << "Failed loading shader files" << std::endl;
	}
	const GLchar *vS = vertexContent.c_str();
	const GLchar *fS = fragmentContent.c_str();
	
	//Creates shader from file data
	ShaderHelperClass shader;
	shader.Compile(vS, fS);
	return shader;
}
//Reads texture file
TextureHelperClass ResHelperClass::parseTextureFiles(const GLchar *file, GLboolean alpha)
{
	
	TextureHelperClass texture;
	if (alpha)
	{
		texture.internalF = GL_RGBA;
		texture.imageF = GL_RGBA;
	}
	//SOIL loading image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.imageF == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	texture.GenerateTex(width, height, image);
	SOIL_free_image_data(image);
	return texture;
}