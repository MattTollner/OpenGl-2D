#pragma once
#include <GL\glew.h>
#include <glm\glm\glm.hpp>
#include "TextureHelperClass.h"
#include "SpriteHelperClass.h"
#include "Animal.h"

class Predator : public Animal
{
public:
	Predator();
	Predator(glm::vec2 pos, TextureHelperClass sprite, GLuint id);
	~Predator();
	glm::vec2 MoveTo();
};

