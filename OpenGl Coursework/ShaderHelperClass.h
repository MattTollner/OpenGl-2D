#pragma once
#include <string>

#include <GL\glew.h>
#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\type_ptr.hpp>



// General purpsoe shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
class ShaderHelperClass
{
public:
	// State
	GLuint ID;
	// Constructor
	ShaderHelperClass() { }
	// Sets the current shader as active
	ShaderHelperClass  &Use();
	// Compiles the shader from given source code
	void    Compile(const GLchar *vertexSource, const GLchar *fragmentSource); 
																													   // Utility functions
	//void    SetFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
	void    SetInteger(const GLchar *name, GLint value, GLboolean useShader = false);
	//void    SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void    SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
	void    SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
private:
	// Checks if compilation or linking failed and if so, print the error logs
	void    checkShaderError(GLuint object, std::string type);
};