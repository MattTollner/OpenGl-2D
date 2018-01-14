#pragma once
#include <GL\glew.h>
#include <glm\glm\glm.hpp>
#include "TextureHelperClass.h"
#include "SpriteHelperClass.h"
#include "Animal.h"

class Prey : public Animal
{
public:
	Prey();
	Prey(glm::vec2 pos, TextureHelperClass sprite, GLuint id);
	~Prey();
};

