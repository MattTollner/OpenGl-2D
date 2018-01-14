#include "Grass.h"



Grass::Grass()
{
}

Grass::Grass(glm::vec2 pos, glm::vec2 size, TextureHelperClass sprite)
{
	this->Position = pos;
	this->Size = size;
	this->Colour = glm::vec3(0.0f, 1.0f, 0.0f);
	this->Sprite = sprite;
	this->Rotation = 79.0f;

}


Grass::~Grass()
{
}

void Grass::DecreaseGrass(GLfloat amount)
{
	this->grassLevel -= amount;
	//glm::vec3 oldColour = this->Colour;
	this->Colour.g -= 0.031f;
}



void Grass::Draw(SpriteHelperClass &renderer)
{
	renderer.RenderSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Colour);
}
