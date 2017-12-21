#include "GameObject.h"
#include <iostream>
#include <iomanip>
using namespace std;


GLfloat  speed = 1;
GLfloat  rot, radian;
glm::vec2 dis;


GameObject::GameObject()
	: Position(0, 0), Size(1, 1), Velocity(0.0f), Color(.5f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) 
{
	std::cout << " NO 1 " << std::endl;
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
	: Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) 
{ 

	
	GLuint randNum;
	GLuint randNum2;

	randNum = rand() % 800 + 1;
	randNum2 = rand() % 600 + 1;

	NewPos = glm::vec2(randNum, randNum2);
}

void GameObject::Draw(SpriteRenderer &renderer)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

glm::vec2 GameObject::MoveTo()
{	
	
	dis.x = NewPos.x - this->Position.x;
	dis.y = NewPos.y - this->Position.y;

	if (sqrt(dis.y*dis.y + dis.x * dis.x) < speed)
	{
		this->Position.x = NewPos.x;
		this->Position.y = NewPos.y;

		GLuint randNum;
			GLuint randNum2;

			randNum = rand() % 800 + 1;
			randNum2 = rand() % 600 + 1;

			NewPos = glm::vec2(randNum, randNum2);
	}
	else {
		radian = atan2(dis.y, dis.x);
		this->Position.x += cos(radian)*speed;
		this->Position.y += sin(radian)*speed;
		rot = radian * 180 / glm::pi<float>();
		this->Rotation = rot;
				
	}

	
	return this->Position;
}

