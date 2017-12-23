#pragma once
#include <glew.h>
#include <glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"
class Grass
{
public:
	GLfloat grassLevel = 30;
	glm::vec2 Position, Size;
	glm::vec3 Colour;
	GLfloat Rotation;
	Texture2D Sprite;
	Grass();
	Grass(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
	~Grass();

	void DecreaseGrass(GLfloat amount);

	void Draw(SpriteRenderer & renderer);
	
};

