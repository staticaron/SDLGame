#include "Level.h"
#include "Bat.h"
#include "Ball.h"

Level::Level()
: m_EntityManager(
{2, 1, Vector2(250, 558), Vector2(2, 2)}, 
{0, 0, Vector2(100, 100), Vector2(2, 2)}
) {}

Level::~Level() {}

void Level::InitColliders(const TextureManager& textureManager)
{
	m_EntityManager.GetBall().InitColliders(textureManager);
	m_EntityManager.GetBat().InitColliders(textureManager);
}

void Level::HandleCollisions() 
{
	auto entities = m_EntityManager.GetEntities();

	for(int x = 0; x < entities.size(); x++)
	{
		for (int y = 0; y < entities.size(); y++) 
		{
			if(x == y) continue;

			entities[x].get().HandleCollisions(entities[y]);
		}
	}
}

void Level::Update(double deltaTime, const InputManager& inputManager)
{
	m_EntityManager.GetBat().Update(deltaTime, inputManager);
	m_EntityManager.GetBall().Update(deltaTime, inputManager);
}

void Level::Render(SDL_Renderer* renderer, const TextureManager& textureManager)
{
	m_EntityManager.GetBat().Render(renderer, textureManager);
	m_EntityManager.GetBall().Render(renderer, textureManager);
}
