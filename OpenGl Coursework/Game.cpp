#include "Game.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "BallObject.h"
#include "Animal.h"
#include "Predator.h"
#include "Prey.h"
#include "Grass.h"
#include <iostream>
using namespace std;


#include <cstdlib>

// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;
BallObject      *Ball;
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
	delete Player;
	delete Ball;
}

void Game::Init()
{
	// Load shaders
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// Load textures
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
	// Set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	// Load levels
	GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;
	// Configure game objects
	glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
	for (unsigned int i = 0; i < 1; i++)
	{
		GLuint randNum;
		GLuint randNum2;

		randNum = rand() % 800 + 1;
		randNum2 = rand() % 600 + 1;
		//std::cout << randNum << " : " << randNum2 << std::endl;

		objs.push_back(GameObject(glm::vec2(randNum,randNum2), glm::vec2(20,20), ResourceManager::GetTexture("face")));
	}


	for (unsigned int i = 0; i < 10; i++)
	{
		GLuint randNum;
		GLuint randNum2;

		randNum = rand() % 800 + 1;
		randNum2 = rand() % 600 + 1;
		std::cout << "Grass  : " << randNum << " : " << randNum2 << std::endl;

		grass.push_back(Grass(glm::vec2(randNum, randNum2), glm::vec2(50,50), ResourceManager::GetTexture("block")));		
	}

	for (unsigned int i = 0; i < 1; i++)
	{
		GLuint randNum;
		GLuint randNum2;

		randNum = rand() % 800 + 1;
		randNum2 = rand() % 600 + 1;
		std::cout << "Grass  : " << randNum << " : " << randNum2 << std::endl;

		animals.push_back(Predator(glm::vec2(randNum, randNum2), ResourceManager::GetTexture("face"), idCount));
		idCount++;
	}

	for (unsigned int i = 0; i < 10; i++)
	{
		GLuint randNum;
		GLuint randNum2;

		randNum = rand() % 800 + 1;
		randNum2 = rand() % 600 + 1;
		std::cout << "Prey  : " <<randNum << " : " << randNum2 << std::endl;

		animals.push_back(Prey(glm::vec2(randNum, randNum2), ResourceManager::GetTexture("face"), idCount));
		idCount++;
	}
}

void Game::Update(GLfloat dt)
{
	// Update objects
	Ball->Move(dt, this->Width);
	// Check for collisions
	this->DoCollisions();
	// Check loss condition
	if (Ball->Position.y >= this->Height) // Did ball reach bottom edge?
	{
		this->ResetLevel();
		this->ResetPlayer();
	}

	for (auto attack = objs.begin(); attack != objs.end(); ++attack)	{
		
		attack->MoveTo();
	}

	for (auto animal = animals.begin(); animal != animals.end(); ++animal)
	{
		animal->MoveTo();
		GLfloat num = RandomNumberInt(3, 2);

		animal->DecraseHunger(num / RandomNumberInt(200, 200));			
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
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// Move playerboard
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x -= velocity;
				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
		{
			Ball->Stuck = false;

			for (auto animal = animals.begin(); animal != animals.end(); ++animal)
			{
				animal->NewPos = glm::vec2(400.0f, 300.0f);
			}
		}
				

	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		// Draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		// Draw level
		this->Levels[this->Level].Draw(*Renderer);
		// Draw player
		Player->Draw(*Renderer);

		
		for (auto attack = objs.begin(); attack != objs.end(); ++attack)
		{
			attack->Draw(*Renderer);
		}

		for (auto animal = animals.begin(); animal != animals.end(); ++animal)
		{
			animal->Draw(*Renderer);
		}

		for (auto grasses = grass.begin(); grasses != grass.end(); ++grasses)
		{
			grasses->Draw(*Renderer);
		}
			
		// Draw ball
		Ball->Draw(*Renderer);
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
	// Reset player/ball stats
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}



// Collision detection
//GLboolean CheckCollision(GameObject &one, GameObject &two);
GLboolean CheckCollision(Animal &one, Animal &two);
GLboolean CheckCollision(Animal &one, Grass &two);
//GLboolean CheckCollision(std::vector<Animal> &one, std::vector<Animal> &two);
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 closest);
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
						if (animal.Breed())
						{
							if (animal.isReady && animal2.isReady)
							{
								GLuint randNum;
								GLuint randNum2;

								randNum = rand() % 800 + 1;
								randNum2 = rand() % 600 + 1;
								animals.push_back(Prey(glm::vec2(randNum, randNum2), ResourceManager::GetTexture("face"), idCount));
								idCount++;
								animal.isReady = false;
								animal2.isReady = false;
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


						//GLuint stupidCount = 2;
						//if (animal.id < 10 && animal2.id < 10)
						//{
						//	if (stupidCount % 2 == 0) 
						//	{
						//		if (animal.Breed())
						//		{
						//			if (animal.isReady && animal2.isReady)
						//			{
						//				GLuint randNum;
						//				GLuint randNum2;

						//				randNum = rand() % 800 + 1;
						//				randNum2 = rand() % 600 + 1;
						//				animals.push_back(Predator(glm::vec2(randNum, randNum2), ResourceManager::GetTexture("face"), idCount));
						//				idCount++;
						//				animal.isReady = false;
						//				animal2.isReady = false;
						//				std::cout << animal.id << " and " << animal2.id << " Predator Spawned" << "  id count at " << idCount << std::endl;								
						//			}

						//		}								
						//	}
						//	stupidCount+=2;
						//}
					}
				}		
			}
		}
	}

	
	


	for (GameObject &box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			if (std::get<0>(collision)) // If collision is true
			{
				// Destroy block if not solid
				if (!box.IsSolid)
					box.Destroyed = GL_TRUE;
				// Collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT) // Horizontal collision
				{
					Ball->Velocity.x = -Ball->Velocity.x; // Reverse horizontal velocity
														  // Relocate
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						Ball->Position.x += penetration; // Move ball to right
					else
						Ball->Position.x -= penetration; // Move ball to left;
				}
				else // Vertical collision
				{
					Ball->Velocity.y = -Ball->Velocity.y; // Reverse vertical velocity
														  // Relocate
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
					if (dir == UP)
						Ball->Position.y -= penetration; // Move ball bback up
					else
						Ball->Position.y += penetration; // Move ball back down
				}
			}
		}
	}
	// Also check collisions for player pad (unless stuck)
	Collision result = CheckCollision(*Ball, *Player);
	if (!Ball->Stuck && std::get<0>(result))
	{
		// Check where it hit the board, and change velocity based on where it hit the board
		GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
		GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
		GLfloat percentage = distance / (Player->Size.x / 2);
		// Then move accordingly
		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		//Ball->Velocity.y = -Ball->Velocity.y;
		Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
																					// Fix sticky paddle
		Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
	}
}



GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
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



Collision CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
	// Get center point circle first 
	glm::vec2 center(one.Position + one.Radius);
	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// Now retrieve vector between center circle and closest point AABB and check if length < radius
	difference = closest - center;

	if (glm::length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	else
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}




// Calculates which direction a vector is facing (N,E,S or W)
Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}




