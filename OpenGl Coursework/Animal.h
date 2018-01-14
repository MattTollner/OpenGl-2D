#pragma once
#include <GL\glew.h>
#include <glm\glm\glm.hpp>
#include "TextureHelperClass.h"
#include "SpriteHelperClass.h"



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
	TextureHelperClass Sprite;

	
	Animal();
	Animal(glm::vec2 pos, glm::vec2 size, TextureHelperClass sprite);
	~Animal();

	glm::vec2 MoveTo(GLfloat dt, GLuint screenWidth , GLuint screenHeight );

	void Draw(SpriteHelperClass & renderer);
	void DecraseHunger(GLfloat decraseAmount);
	void DecreaseFertility(GLfloat decreaseAmount);
	GLboolean Breed();	

};

