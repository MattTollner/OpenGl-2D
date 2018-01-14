
#include "SpriteHelperClass.h"

SpriteHelperClass::SpriteHelperClass(ShaderHelperClass &shader)
{
	this->shader = shader;
	this->quadSetup();
}

SpriteHelperClass::~SpriteHelperClass()
{
	glDeleteVertexArrays(1, &this->qVAO);
}

void SpriteHelperClass::RenderSprite(TextureHelperClass &texture, glm::vec2 position, glm::vec2 scale, GLfloat rotate, glm::vec3 colour)
{
	
	this->shader.UseShader();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(position, 0.0f));  

	// Centers origin for rotation
	model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); 
	//Resets origin to top left
	model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f)); 
	model = glm::scale(model, glm::vec3(scale, 1.0f)); 

	this->shader.SetMatrix4("model", model);

	// Render textured quad
	this->shader.SetVector3f("spriteColor", colour);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(this->qVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
#
//Creates quad 
void SpriteHelperClass::quadSetup()
{

	GLfloat vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f, //tri 
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	GLuint VBO;
	glGenVertexArrays(1, &this->qVAO); //Creates 1 vertex array object
	glGenBuffers(1, &VBO); // Creates 1 vertex buffer

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->qVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}