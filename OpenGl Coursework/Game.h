#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <cstdlib>
#include <vector>

//Game modes
enum GameMode {
	GAME,
	MENU
};

class Game
{
public:
	GameMode              State;
	GLboolean              Keys[1024];
	GLboolean KeysProcessed[1024];
	GLuint                 Width, Height;

	Game(GLuint width, GLuint height);
	~Game();
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
	void CheckForCollisions();
	// Reset
	void SetScreenSize(GLuint screenWidth, GLuint screenHeight);
	
};

