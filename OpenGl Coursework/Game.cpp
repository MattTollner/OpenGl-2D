#include "Game.h"
#include "ResHelperClass.h"
#include "TextHelperClass.h"
#include "Animal.h"
#include "Predator.h"
#include "Prey.h"
#include "Grass.h"
#include <irrKlang\irrKlang.h>
#include <iostream>
using namespace std;
using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();
#include <cstdlib>

// Game-related State data
SpriteHelperClass  *Renderer;
TextHelperClass  *Text;
std::vector<Animal> animals;
std::vector<Grass> grass;
std::vector<Predator> predators;
std::vector<Prey> prey;
GLuint idCount = 0;
GLboolean gameChange = false;

// The Width of the screen
 GLuint SCREEN_WIDTH = 800;
// The height of the screen
 GLuint SCREEN_HEIGHT = 800;

void DeleteAnimal(Animal  animalToDel)
{
	
	GLuint cnt = 0;

	for (auto& isAnimal : animals)
	{
		if (isAnimal.id == animalToDel.id)
		{
			break;
		}
		cnt++;
	}
	SoundEngine->play2D("sounds/roar.wav", GL_FALSE);
	//Deletes pray from list
	animals.erase(animals.begin() + cnt);
}

GLuint RandomNumberInt(GLuint numLarge, GLuint  numSmall)
{
	GLuint randNum;

	randNum = rand() % numLarge + numSmall;

	return randNum;

}



Game::Game(GLuint width, GLuint height)
	: State(MENU), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	delete Renderer;
	delete Text;
}

void Game::Init()
{
	//Memory Leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	
	
	ResHelperClass::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", "sprite");
	//Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	//Sets uniforms in shaders
	ResHelperClass::GetShader("sprite").UseShader().SetInt("sprite", 0);
	ResHelperClass::GetShader("sprite").SetMatrix4("projection", projection);

	ResHelperClass::LoadTexture("textures/sandBG.jpg", GL_FALSE, "backgroundGame");
	ResHelperClass::LoadTexture("textures/grass.jpg", GL_FALSE, "backgroundMenu");
	ResHelperClass::LoadTexture("textures/grass.jpg", GL_FALSE, "grass");
	ResHelperClass::LoadTexture("textures/tiger.png",	GL_TRUE, "tiger");
	ResHelperClass::LoadTexture("textures/prey.png", GL_TRUE, "prey");

	//Text renderer
	Text = new TextHelperClass(600, 600);
	Text->LoadText("fonts/Organo.ttf", 24);
	Renderer = new SpriteHelperClass(ResHelperClass::GetShader("sprite"));


	for (unsigned int i = 0; i < 15; i++)
	{
		GLuint randNum;
		GLuint randNum2;

		randNum = rand() % SCREEN_WIDTH + 1;
		randNum2 = rand() % SCREEN_HEIGHT + 1;
		std::cout << "Grass  : " << randNum << " : " << randNum2 << std::endl;

		grass.push_back(Grass(glm::vec2(randNum, randNum2), glm::vec2(50,50), ResHelperClass::GetTexture("grass")));		
	}

	for (unsigned int i = 0; i < 1; i++)
	{
		GLuint randNum;
		GLuint randNum2;

		randNum = rand() % SCREEN_WIDTH + 1;
		randNum2 = rand() % SCREEN_HEIGHT + 1;
		std::cout << "Predator  : " << randNum << " : " << randNum2 << std::endl;

		animals.push_back(Predator(glm::vec2(randNum, randNum2), ResHelperClass::GetTexture("tiger"), idCount));
		idCount++;
	}

	for (unsigned int i = 0; i < 10; i++)
	{
		GLuint randNum;
		GLuint randNum2;

		randNum = rand() % SCREEN_WIDTH + 1;
		randNum2 = rand() % SCREEN_HEIGHT + 1;
		std::cout << "Prey  : " <<randNum << " : " << randNum2 << std::endl;

		animals.push_back(Prey(glm::vec2(randNum, randNum2), ResHelperClass::GetTexture("prey"), idCount));
		idCount++;
	}
}

void Game::Update(GLfloat deltaTime)
{

	// Check for collisions
	this->CheckForCollisions();

	

	for (auto animal = animals.begin(); animal != animals.end(); ++animal)
	{
		animal->MoveTo(deltaTime, SCREEN_WIDTH, SCREEN_HEIGHT);
		GLfloat num = RandomNumberInt(3, 2);

		animal->DecraseHunger((num/10)*deltaTime);		
		animal->DecreaseFertility(4 * deltaTime);
	}

	unsigned i = 0;
	while (i < animals.size())
	{
		if (animals[i].Hunger <= 0)
		{
			animals.erase(animals.begin() + i);
		}
		else {
			++i;
		}
	}	

	unsigned e = 0;
	while (e < grass.size())
	{
		if (grass[e].grassLevel <= 0)
		{
			grass.erase(grass.begin() + e);
		}
		else {
			++e;
		}
	}
		
}


void Game::ProcessInput(GLfloat deltaTime)
{
	if (this->State == GAME)	{
		
		
		if (this->Keys[GLFW_KEY_SPACE])
		{		

			for (auto animal = animals.begin(); animal != animals.end(); ++animal)
			{
				animal->NewPos = glm::vec2(400.0f, 300.0f);
			}
		}

		if (this->Keys[GLFW_KEY_P] && !this->KeysProcessed[GLFW_KEY_P])
		{
			GLuint randNum;
			GLuint randNum2;

			randNum = rand() % SCREEN_WIDTH + 1;
			randNum2 = rand() % SCREEN_HEIGHT + 1;

			animals.push_back(Prey(glm::vec2(randNum, randNum2), ResHelperClass::GetTexture("prey"), idCount));
			idCount++;
			this->KeysProcessed[GLFW_KEY_P] = GL_TRUE;
		}

		if (this->Keys[GLFW_KEY_O] && !this->KeysProcessed[GLFW_KEY_O])
		{			

			for (auto& animals : animals)
			{
				if (animals.isPrey)
				{
					DeleteAnimal(animals);
					break;
				}
			}
			this->KeysProcessed[GLFW_KEY_O] = GL_TRUE;
		}

		if (this->Keys[GLFW_KEY_L] && !this->KeysProcessed[GLFW_KEY_L])
		{
			GLuint randNum;
			GLuint randNum2;

			randNum = rand() % SCREEN_WIDTH + 1;
			randNum2 = rand() % SCREEN_HEIGHT + 1;

			animals.push_back(Predator(glm::vec2(randNum, randNum2), ResHelperClass::GetTexture("tiger"), idCount));
			idCount++;
			this->KeysProcessed[GLFW_KEY_L] = GL_TRUE;
		}


		if (this->Keys[GLFW_KEY_K] && !this->KeysProcessed[GLFW_KEY_K])
		{
			for (auto& animala : animals)
			{
				if (!animala.isPrey)
				{
					DeleteAnimal(animala);
					break;
				}
			}
		
			this->KeysProcessed[GLFW_KEY_K] = GL_TRUE;
		}

		if (this->Keys[GLFW_KEY_R] && !this->KeysProcessed[GLFW_KEY_R])
		{

			for (unsigned int i = 0; i < 5; i++)
			{
				GLuint randNum;
				GLuint randNum2;

				randNum = rand() % SCREEN_WIDTH + 1;
				randNum2 = rand() % SCREEN_HEIGHT + 1;
				std::cout << "Grass  : " << randNum << " : " << randNum2 << std::endl;

				grass.push_back(Grass(glm::vec2(randNum, randNum2), glm::vec2(50, 50), ResHelperClass::GetTexture("grass")));
				SoundEngine->play2D("sounds/RainSound.wav", GL_FALSE);
			}

			this->KeysProcessed[GLFW_KEY_R] = GL_TRUE;
		}

		if (this->Keys[GLFW_KEY_E] && !this->KeysProcessed[GLFW_KEY_E])	
		{	
			grass.pop_back();	
			this->KeysProcessed[GLFW_KEY_E] = GL_TRUE;
		}
		
	}

	if (this->Keys[GLFW_KEY_M] && !this->KeysProcessed[GLFW_KEY_M])
	{
		
		if (this->State == GAME)
		{
			this->State = MENU;
		}		
		else
		{
			this->State = GAME;
		}
		this->KeysProcessed[GLFW_KEY_M] = GL_TRUE;
	}
}

void Game::Render()
{
	GLuint predCount = 0;
	GLuint preyCount = 0;
	if (this->State == GAME)
	{
		// Draw game background
		Renderer->RenderSprite(ResHelperClass::GetTexture("backgroundGame"), glm::vec2(0, 0), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), 0.0f);
		
		for (auto animal = animals.begin(); animal != animals.end(); ++animal)
		{
			animal->Draw(*Renderer);
			if (animal->isPrey)
			{
				preyCount++;
			}
			else
			{
				predCount++;
			}
		}

		for (auto grasses = grass.begin(); grasses != grass.end(); ++grasses)
		{
			grasses->Draw(*Renderer);
		}	

		
		auto predString = std::to_string(predCount);
		auto preyString = std::to_string(preyCount);

		Text->RenderText("Prey : " + preyString + " Pred : " + predString  , 5.0f, 5.0f, 1.0f);
		Text->RenderText("Menu : M", Width/4, Width/1.5f,  1.0f);
		

		
	}

	if (this->State == MENU)
	{
		// Draw menu background
		Renderer->RenderSprite(ResHelperClass::GetTexture("backgroundMenu"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);

		Text->RenderText("Artificial Life Simulation", Width / 4 - 75, Height / 4, 1.3f);
		Text->RenderText("P to spawn Prey  O to remove Prey", Width / 4 - 60, Height / 3, 0.75f);
		Text->RenderText("L to spawn Pred  K to remove Pred", Width / 4 - 60, Height / 3 + 20, 0.75f);
		Text->RenderText("R make it rain   E to remove food", Width / 4 - 60, Height / 3 + 40, 0.75f);
		Text->RenderText("M to begin", Width / 4 - 120, Height / 3 + 60, 0.75f);
	}

}


void Game::SetScreenSize(GLuint screenWidth, GLuint screenHeight)
{
	SCREEN_WIDTH = screenWidth;
	SCREEN_HEIGHT = screenHeight;
	//cout << "Width " << screenWidth << " Heigth " << screenHeight;
}



// Collision detection

GLboolean CheckCollision(Animal &one, Animal &two);
GLboolean CheckCollision(Animal &one, Grass &two);
void DeleteAnimal(Animal animalToDel);
GLuint RandomNumberInt(GLuint numLarge, GLuint numSmall);

void Game::CheckForCollisions()
{

	for (auto& animal : animals)
	{	
		for (auto& grasses : grass)
		{
			if (CheckCollision(animal, grasses))
			{
				if (animal.isPrey)
				{
					if (animal.Hunger < 10)
					{
						animal.Hunger += 0.25f;
						grasses.DecreaseGrass(1);					
					}
				}
			}
		}

		for (auto& animal2 : animals) 
		{
			if (CheckCollision(animal,animal2))
			{
				if (animal.id != animal2.id)
				{
					if (animal.isPrey == true && animal2.isPrey == true)
					{
						//std::cout << "Potential Breed between Prey" << animal.id << " and " << animal2.id <<  std::endl;
						if (animal.isFemale != animal2.isFemale)
						{
							if (animal.Fertile <= 0 && animal2.Fertile <= 0)
							{
								GLuint randNum;
								GLuint randNum2;

								randNum = rand() % 800 + 1;
								randNum2 = rand() % 600 + 1;
								animals.push_back(Prey(glm::vec2(randNum, randNum2), ResHelperClass::GetTexture("prey"), idCount));
								SoundEngine->play2D("sounds/prey.wav", GL_FALSE);
								idCount++;

								animal.Fertile = 30.0f;
								animal2.Fertile = 30.0f;
								std::cout << animal.id << " and " << animal2.id << " Prey Spawned" << std::endl;
							}

						}

					}
					if (animal.isPrey == true && animal2.isPrey == false)
					{
						if (animal2.Hunger < 6)
						{
							animal2.Hunger = 10;
							DeleteAnimal(animal);
							
							break;
						}
						
					}
					if (animal.isPrey == false && animal2.isPrey == false)
					{
						//Canabalism 
						if (animal.Hunger < 2 || animal2.Hunger < 2)
						{
							GLuint num = RandomNumberInt(2, 1);
							if (num == 1)
							{
								std::cout << "Animal One gonna kill animal two" << std::endl;
								animal.Hunger = 10;
								DeleteAnimal(animal2);								
								break;
							}
							else
							{
								std::cout << "Animal Two gonna kill animal One" << std::endl;
								animal2.Hunger = 10;
								DeleteAnimal(animal);					
								break;
							}
						}


								if (animal.isFemale != animal2.isFemale)
								{
									if (animal.Fertile <= 0 && animal2.Fertile <= 0)
									{
										GLuint randNum;
										GLuint randNum2;

										randNum = rand() % 800 + 1;
										randNum2 = rand() % 600 + 1;
										animals.push_back(Predator(glm::vec2(randNum, randNum2), ResHelperClass::GetTexture("tiger"), idCount));
										SoundEngine->play2D("sounds/predator.wav", GL_FALSE);
										idCount++;
										animal.Fertile = 30.0f;
										animal2.Fertile = 30.0f;
										std::cout << animal.id << " and " << animal2.id << " Predator Spawned" << "  id count at " << idCount << std::endl;								
									}

								}					
					
					}
				}		
			}
		}
	}
	
	
}



GLboolean CheckCollision(Animal & one, Animal & two)
{
	// Collision x-axis?
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// Collision y-axis?
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// Collision only if on both axes
	return collisionX && collisionY;
}

GLboolean CheckCollision(Animal & one, Grass & two)
{
	// Collision x-axis?
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// Collision y-axis?
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// Collision only if on both axes
	return collisionX && collisionY;
}

void renderBitmapString(float x, float y, char *string)
{
	
}









