#pragma once
#include <glew.h>
#include <glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "Animal.h"

class Prey : public Animal
{
public:
	Prey();
	Prey(glm::vec2 pos, Texture2D sprite, GLuint id);
	~Prey();
};

