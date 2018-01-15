
#include "SpriteHelperClass.h"
//Init setup
SpriteHelperClass::SpriteHelperClass(ShaderHelperClass &shader)
{
	this->shader = shader;
	this->quadSetup();
}

//Clean up
SpriteHelperClass::~SpriteHelperClass()
{
	glDeleteVertexArrays(1, &this->qVAO);
}

//Deals with matrix transformations then renders textured quad to screen
void SpriteHelperClass::RenderSprite(TextureHelperClass &texture, glm::vec2 position, glm::vec2 scale, GLfloat rotate, glm::vec3 colour)
{
	
	this->shader.UseShader();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));  

	//Centers origin for rotation
	model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); 
	//Resets origin to top left
	model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f)); 
	model = glm::scale(model, glm::vec3(scale, 1.0f)); 

	this->shader.SetMatrix4("model", model);

	//Renders textured quad
	this->shader.SetVector3f("spriteColour", colour);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	//Draws textured quad
	glBindVertexArray(this->qVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

//Creates quad that sprites sit on
void SpriteHelperClass::quadSetup()
{
	//Pos,TexCoords
	GLfloat vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f, 
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	GLuint VBO;
	glGenVertexArrays(1, &this->qVAO); //Creates 1 vertex array object
	glGenBuffers(1, &VBO); //Creates 1 vertex buffer

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->qVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}