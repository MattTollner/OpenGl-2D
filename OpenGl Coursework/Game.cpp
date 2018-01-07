#include "Game.h"
#include "ResourceManager.h"
#include "TextRenderer.h"
#include "GameObject.h"
#include "BallObject.h"
#include "Animal.h"
#include "Predator.h"
#include "Prey.h"
#include "Grass.h"
#include <irrKlang.h>
#include <iostream>
using namespace std;
using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();
#include <cstdlib>

// Game-related State data
SpriteRenderer  *Renderer;
TextRenderer  *Text;
std::vector<GameObject> objs;
std::vector<Animal> animals;
std::vector<Grass> grass;
std::vector<Predator> predators;
std::vector<Prey> prey;
GLuint idCount = 0;

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
	SoundEngine->play2D("audio/roar.wav", GL_FALSE);
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
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	delete Renderer;
	delete Text;
}

void Game::Init()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	
	// Load shaders
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("textures/sandBG.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("textures/grass.jpg", GL_FALSE, "grass");
	ResourceManager::LoadTexture("textures/tiger.png",	GL_TRUE, "tiger");
	ResourceManager::LoadTexture("textures/prey.png", GL_TRUE, "prey");

	//Text renderer
	Text = new TextRenderer(600, 600);
	Text->Load("font/Organo.ttf", 24);

	//Audio Load
	//SoundEngine->play2D("audio/breakout.mp3", GL_TRUE);


	// Set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	// Load levels
	
	// Configure game objects	

	for (unsigned int i = 0; i < 15; i++)
	{
		GLuint randNum;
		GLuint randNum2;

		randNum = rand() % 800 + 1;
		randNum2 = rand() % 600 + 1;
		std::cout << "Grass  : " << randNum << " : " << randNum2 << std::endl;

		grass.push_back(Grass(glm::vec2(randNum, randNum2), glm::vec2(50,50), ResourceManager::GetTexture("grass")));		
	}

	for (unsigned int i = 0; i < 1; i++)
	{
		GLuint randNum;
		GLuint randNum2;

		randNum = rand() % 800 + 1;
		randNum2 = rand() % 600 + 1;
		std::cout << "Predator  : " << randNum << " : " << randNum2 << std::endl;

		animals.push_back(Predator(glm::vec2(randNum, randNum2), ResourceManager::GetTexture("tiger"), idCount));
		idCount++;
	}

	for (unsigned int i = 0; i < 10; i++)
	{
		GLuint randNum;
		GLuint randNum2;

		randNum = rand() % 800 + 1;
		randNum2 = rand() % 600 + 1;
		std::cout << "Prey  : " <<randNum << " : " << randNum2 << std::endl;

		animals.push_back(Prey(glm::vec2(randNum, randNum2), ResourceManager::GetTexture("prey"), idCount));
		idCount++;
	}
}

void Game::Update(GLfloat dt)
{

	// Check for collisions
	this->DoCollisions();
	// Check loss condition
	//if (Ball->Position.y >= this->Height) // Did ball reach bottom edge?
	//{
	//	this->ResetLevel();
	//	this->ResetPlayer();
	//}

	for (auto attack = objs.begin(); attack != objs.end(); ++attack)	{
		
		attack->MoveTo();
	}

	for (auto animal = animals.begin(); animal != animals.end(); ++animal)
	{
		animal->MoveTo();
		GLfloat num = RandomNumberInt(3, 2);

		animal->DecraseHunger(num / RandomNumberInt(200, 200));		
		animal->DecreaseFertility(0.05f);
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


void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)	{
		
		
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

			randNum = rand() % 800 + 1;
			randNum2 = rand() % 600 + 1;

			animals.push_back(Prey(glm::vec2(randNum, randNum2), ResourceManager::GetTexture("prey"), idCount));
			this->KeysProcessed[GLFW_KEY_P] = GL_TRUE;
		}

		if (this->Keys[GLFW_KEY_O] && !this->KeysProcessed[GLFW_KEY_O])
		{			

			animals.pop_back();
			this->KeysProcessed[GLFW_KEY_O] = GL_TRUE;
		}

		if (this->Keys[GLFW_KEY_L] && !this->KeysProcessed[GLFW_KEY_L])
		{
			GLuint randNum;
			GLuint randNum2;

			randNum = rand() % 800 + 1;
			randNum2 = rand() % 600 + 1;

			animals.push_back(Predator(glm::vec2(randNum, randNum2), ResourceManager::GetTexture("tiger"), idCount));
			this->KeysProcessed[GLFW_KEY_L] = GL_TRUE;
		}


		if (this->Keys[GLFW_KEY_K] && !this->KeysProcessed[GLFW_KEY_K])
		{

			animals.pop_back();
			this->KeysProcessed[GLFW_KEY_K] = GL_TRUE;
		}

		

	}
}

void Game::Render()
{
	GLuint predCount = 0;
	GLuint preyCount = 0;
	if (this->State == GAME_ACTIVE)
	{
		// Draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		
		
		/*for (auto attack = objs.begin(); attack != objs.end(); ++attack)
		{
			attack->Draw(*Renderer);
		}*/

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

		Text->RenderText("Artificial Life Simulation", Width/4 - 75, Height / 4, 1.3f);
		Text->RenderText("P to spawn Prey  O to remove Prey", Width / 4 - 60, Height / 3, 0.75f);
		Text->RenderText("L to spawn Pred  K to remove Pred", Width / 4 - 60, Height / 3 + 20, 0.75f);
		Text->RenderText("R to make it rain", Width / 4 + 35 , Height / 3 + 40, 0.75f);

		
	}

	if (this->State == GAME_MENU)
	{
		// Draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);

		Text->RenderText("Artificial Life Simulation", 250.0f, Height / 2, 1.0f);
		Text->RenderText("P to spawn Prey | O to remove Prey", 245.0f, Height / 2 + 20.0f, 0.75f);
		Text->RenderText("L to spawn Predator| K to remove Predator", 245.0f, Height / 2 + 20.0f, 0.75f);
	}

}


void Game::ResetLevel()
{
	if (this->Level == 0)this->Levels[0].Load("levels/one.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 1)
		this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 2)
		this->Levels[2].Load("levels/three.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 3)
		this->Levels[3].Load("levels/four.lvl", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
	//// Reset player/ball stats
	//Player->Size = PLAYER_SIZE;
	//Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	//Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}



// Collision detection

GLboolean CheckCollision(Animal &one, Animal &two);
GLboolean CheckCollision(Animal &one, Grass &two);
void DeleteAnimal(Animal animalToDel);
GLuint RandomNumberInt(GLuint numLarge, GLuint numSmall);

void Game::DoCollisions()
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
								animals.push_back(Prey(glm::vec2(randNum, randNum2), ResourceManager::GetTexture("prey"), idCount));
								SoundEngine->play2D("audio/prey.wav", GL_FALSE);
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
										animals.push_back(Predator(glm::vec2(randNum, randNum2), ResourceManager::GetTexture("tiger"), idCount));
										SoundEngine->play2D("audio/predator.wav", GL_TRUE);
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









