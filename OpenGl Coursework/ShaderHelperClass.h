#pragma once
#include <string>

#include <GL\glew.h>
#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\type_ptr.hpp>



//Compiles shaders from file
class ShaderHelperClass
{
public:
	GLuint ID;
	ShaderHelperClass() { }
	//Sets current shader to active
	ShaderHelperClass  &Use();
	// Compiles the shader from given source code
	void    Compile(const GLchar *vertexShader, const GLchar *fragmentShader); 

	void    SetInt(const GLchar *name, GLint value, GLboolean useShader = false);
	void    SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
	void    SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
private:
	//Checks for shader errors
	void    checkShaderError(GLuint object, std::string type);
};