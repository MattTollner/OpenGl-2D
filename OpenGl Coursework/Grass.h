#pragma once
#include <GL\glew.h>
#include <glm\glm\glm.hpp>
#include "TextureHelperClass.h"
#include "SpriteHelperClass.h"
class Grass
{
public:
	GLfloat grassLevel = 30;
	glm::vec2 Position, Size;
	glm::vec3 Colour;
	GLfloat Rotation;
	TextureHelperClass Sprite;
	Grass();
	Grass(glm::vec2 pos, glm::vec2 size, TextureHelperClass sprite);
	~Grass();

	void DecreaseGrass(GLfloat amount);

	void Draw(SpriteHelperClass & renderer);
	
};

