#pragma once
#include <glew.h>
#include <glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"
class GameObject
{
public:
	// Object state
	glm::vec2   Position, Size, Velocity, NewPos;
	glm::vec3   Color;

	GLboolean isPrey;
	GLfloat     Rotation;
	GLboolean   IsSolid;
	GLboolean   Destroyed;
	// Render state
	Texture2D   Sprite;
	// Constructor(s)
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	// Draw sprite
	virtual void Draw(SpriteRenderer &renderer);
	glm::vec2 MoveTo();
	
};
