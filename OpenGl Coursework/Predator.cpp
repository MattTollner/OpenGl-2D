#include "Predator.h"
#include <iostream>
#include <iomanip>
using namespace std;



Predator::Predator()
{
	
}

Predator::Predator(glm::vec2 pos, Texture2D sprite) : Animal(pos, glm::vec2(30, 30), sprite)
{
	this->Position = pos;
	this->Sprite = sprite;
	
	Size = glm::vec2(30, 30);
	this->Colour = glm::vec3(1.0f, 0.0f, 0.0f);
	GLuint randNum;
	GLuint randNum2;

	randNum = rand() % 800 + 1;
	randNum2 = rand() % 600 + 1;

	NewPos = glm::vec2(randNum, randNum2);
}


Predator::~Predator()
{
}

glm::vec2 Predator::MoveTo()
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
