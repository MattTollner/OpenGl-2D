#include "ShaderHelperClass.h"
#include <iostream>

//Sets program object as current active shader prog
ShaderHelperClass &ShaderHelperClass::UseShader()
{
	glUseProgram(this->ID);
	return *this;
}

//Compiles & checks for errors then links
void ShaderHelperClass::Compile(const GLchar* vertexShader, const GLchar* fragmentShader)
{
	GLuint vertexShade, fragmentShade;
	
	vertexShade = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShade, 1, &vertexShader, NULL);
	glCompileShader(vertexShade);
	checkShaderError(vertexShade, "Vert");

	fragmentShade = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShade, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShade);
	checkShaderError(fragmentShade, "Frag");
	
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertexShade);
	glAttachShader(this->ID, fragmentShade);
	
	glLinkProgram(this->ID);
	checkShaderError(this->ID, "Prog");
	//Shaders linked delete vars
	glDeleteShader(vertexShade);
	glDeleteShader(fragmentShade);

}

//Sets unifroms values in the shader files
void ShaderHelperClass::SetInt(const GLchar *name, GLint value, GLboolean useShader)
{
	if (useShader)
	{
		this->UseShader();
	}		
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void  ShaderHelperClass::SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader)
{
	if (useShader)
	{
		this->UseShader();
	}		
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

void ShaderHelperClass::SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader)
{
	if (useShader)
	{
		this->UseShader();
	}		
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

//Checks for shader errors
void ShaderHelperClass::checkShaderError(GLuint object, std::string type)
{
	GLint result;
	GLchar infoLog[1024];
	if (type != "Prog")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "Shader error - Compile : Type: " << type << "\n" << infoLog << "\n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "Shader error - Link : Type: " << type << "\n" << infoLog << "\n" << std::endl;
		}
	}
}