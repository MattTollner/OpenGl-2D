#pragma once
#include <map>
#include <string>

#include <GL\glew.h>

#include "TextureHelperClass.h"
#include "ShaderHelperClass.h"


// A static singleton ResHelperClass class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResHelperClass
{
public:
	// Resource storage
	static std::map<std::string,ShaderHelperClass>   Shaders;
	static std::map<std::string, TextureHelperClass> Textures;
	// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	static ShaderHelperClass   LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile,  std::string name);
	// Retrieves a stored sader
	static ShaderHelperClass   GetShader(std::string name);
	// Loads (and generates) a texture from file
	static TextureHelperClass LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
	// Retrieves a stored texture
	static TextureHelperClass GetTexture(std::string name);
	// Properly de-allocates all loaded resources
	static void      Clear();

private:
	// Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResHelperClass() { }
	// Loads and generates a shader from file
	static ShaderHelperClass    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile);
	// Loads a single texture from file
	static TextureHelperClass loadTextureFromFile(const GLchar *file, GLboolean alpha);
};
