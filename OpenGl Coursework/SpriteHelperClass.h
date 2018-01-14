#pragma once
#include <GL\glew.h>
#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>


#include "TextureHelperClass.h"
#include "ShaderHelperClass.h"

class SpriteHelperClass
{
public:
	SpriteHelperClass(ShaderHelperClass &shader);
	~SpriteHelperClass();
	//Renders quad and sprite
	void RenderSprite(TextureHelperClass &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	ShaderHelperClass shader;
	GLuint qVAO;
	//Sets up vertex and buffer attribs
	void quadSetup();
};