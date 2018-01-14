#ifdef _MSC_VER
// Use Visual C++'s memory checking functionality
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _MSC_VER

#define GLEW_STATIC
#include <gl\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Game.h"
#include "ResHelperClass.h"


// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const GLuint SCREEN_WIDTH = 800;
// The height of the screen
const GLuint SCREEN_HEIGHT = 800;




Game Simulation(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
#ifdef _MSC_VER
	
	//_CrtSetBreakAlloc(327);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
#endif // _MSC_VER
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Artificial Life Simulation", nullptr, nullptr);
	
	glfwMakeContextCurrent(window);



	glewExperimental = GL_TRUE;
	glewInit();
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

	glfwSetKeyCallback(window, key_callback);


	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	int w;
	int h;
	
	// Initialize game
	Simulation.Init();


	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// Start Game within Menu State
	Simulation.State = GAME;




	while (!glfwWindowShouldClose(window))
	{

		
		//Delta time calc
		GLfloat curFrame = glfwGetTime();
		deltaTime = curFrame - lastFrame;
		lastFrame = curFrame;
		glfwPollEvents();

	
		glfwGetWindowSize(window, &w, &h);
		
		Simulation.SetScreenSize(w, h);


		Simulation.ProcessInput(deltaTime);

		if (Simulation.State == GAME)
		{
			// Update Game state
			Simulation.Update(deltaTime);
		}
		
		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Simulation.Render();

		glfwSwapBuffers(window);
	}

	// Delete all resources as loaded using the resource manager
	ResHelperClass::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Simulation.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			Simulation.Keys[key] = GL_FALSE;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Simulation.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
		{
			Simulation.Keys[key] = GL_FALSE;
			Simulation.KeysProcessed[key] = GL_FALSE;
		}
	}
}