#include "Level.h"
#include "Bat.h"
#include "Ball.h"

Level::Level()
: m_EntityManager(
{2, 1, glm::vec2(250, 558), glm::vec2(2, 2)}, 
{0, 0, glm::vec2(300, 100), glm::vec2(2, 2)}
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

void Level::RenderImGui()
{
	m_EntityManager.GetBall().RenderImGui();
	m_EntityManager.GetBat().RenderImGui();
}

void Level::Render(SDL_Renderer* renderer, const TextureManager& textureManager)
{
	m_EntityManager.GetBat().Render(renderer, textureManager);
	m_EntityManager.GetBall().Render(renderer, textureManager);
}
