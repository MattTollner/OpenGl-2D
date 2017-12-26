#include "Prey.h"



Prey::Prey()
{
}

Prey::Prey(glm::vec2 pos, Texture2D sprite, GLuint id)
{

	this->Position = pos;
	this->Sprite = sprite;
	this->Colour = glm::vec3(1.0f, 1.0f, 1.0f);
	this->id = id;
	this->isPrey = true;
	


	Size = glm::vec2(30, 30);
	GLuint randNum;
	GLuint randNum2;

	randNum = rand() % 800 + 1;
	randNum2 = rand() % 600 + 1;

	NewPos = glm::vec2(randNum, randNum2);

	randNum = rand() % 2, 1;
	if (randNum == 1)
	{
		this->isFemale = true;
	}
	else
	{
		this->isFemale = false;
	}

}


Prey::~Prey()
{
	
}
