#include "Animal.h"


Animal::Animal()
{
}

Animal::Animal(glm::vec2 pos, glm::vec2 size, Texture2D sprite) : Position(50, 50), Size(1, 1),  Colour(.5f), Rotation(0.0f), Sprite()
{
	
}


Animal::~Animal()
{
}

glm::vec2 Animal::MoveTo()
{

	dis.x = NewPos.x - this->Position.x;
	dis.y = NewPos.y - this->Position.y;

	if (sqrt(dis.y*dis.y + dis.x * dis.x) < speed)
	{
		this->Position.x = NewPos.x;
		this->Position.y = NewPos.y; 

		GLuint randNum;
		GLuint randNum2;

		randNum = rand() % 800 + 1;
		randNum2 = rand() % 600 + 1;

		NewPos = glm::vec2(randNum, randNum2);
	}
	else {
		radian = atan2(dis.y, dis.x);
		this->Position.x += cos(radian)*speed;
		this->Position.y += sin(radian)*speed;
		rot = radian * 180 / glm::pi<float>();
		this->Rotation = rot;
	}

	return this->Position;
}

void Animal::Draw(SpriteRenderer &renderer)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Colour);
}

void Animal::DecraseHunger(GLfloat decraseAmount)
{
	this->Hunger -= decraseAmount;
}

void Animal::DecreaseFertility(GLfloat decreaseAmount)
{
	this->Fertile -= decreaseAmount;
}


GLboolean Animal::Breed()
{
	return true;
}
