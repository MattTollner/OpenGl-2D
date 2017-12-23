#include "Grass.h"



Grass::Grass()
{
}

Grass::Grass(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
{
	this->Position = pos;
	this->Size = size;
	this->Colour = glm::vec3(0.0f, 1.0f, 0.0f);
	this->Sprite = sprite;

}


Grass::~Grass()
{
}

void Grass::DecreaseGrass(GLfloat amount)
{
	this->grassLevel -= amount;
}

void Grass::Draw(SpriteRenderer &renderer)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Colour);
}
