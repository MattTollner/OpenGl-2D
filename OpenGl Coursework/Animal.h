#pragma once
#include <glew.h>
#include <glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"



class Animal
{
public:
	//Atributes
	GLfloat  speed = 1;
	GLfloat  rot, radian;
	GLint id;
	GLfloat Hunger = 10;
	GLfloat Speed;
	GLfloat Fertile = 30;
	GLboolean isFemale;
	glm::vec2 dis;
	GLboolean isPrey = false;
	GLboolean isReady = true;

	glm::vec2 Position, Size, NewPos;
	glm::vec3 Colour;

	GLfloat Rotation;
	Texture2D Sprite;

	
	Animal();
	Animal(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
	~Animal();

	glm::vec2 MoveTo();

	void Draw(SpriteRenderer & renderer);
	void DecraseHunger(GLfloat decraseAmount);
	void DecreaseFertility(GLfloat decreaseAmount);
	GLboolean Breed();


	

};

