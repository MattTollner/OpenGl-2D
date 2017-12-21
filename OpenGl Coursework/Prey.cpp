#include "Prey.h"



Prey::Prey()
{
}

Prey::Prey(glm::vec2 pos, Texture2D sprite)
{
	//this->Colour = glm::vec3(0.0f, 1.0f, 0.0f);
	Size = glm::vec2(50, 50);
	GLuint randNum;
	GLuint randNum2;

	randNum = rand() % 800 + 1;
	randNum2 = rand() % 600 + 1;

	NewPos = glm::vec2(randNum, randNum2);
}


Prey::~Prey()
{
	
}
