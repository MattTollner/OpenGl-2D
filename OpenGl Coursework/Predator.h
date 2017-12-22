#pragma once
#include <glew.h>
#include <glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "Animal.h"

class Predator : public Animal
{
public:
	Predator();
	Predator(glm::vec2 pos, Texture2D sprite, GLuint id);
	~Predator();
	glm::vec2 MoveTo();
};

