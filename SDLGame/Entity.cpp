#include <iostream>

#include "Entity.h"
#include "SDL/SDL_image.h"

Entity::Entity(EntityDetails details) 
: m_EntityDetails(details)
{
	
}

Entity::~Entity() {}

void Entity::InitColliders(const TextureManager& textureManager)
{
	m_EntityBounds = EntityBounds(textureManager, m_EntityDetails);
}

void Entity::Update(double deltaTime, const InputManager& inputManager) {}

void Entity::Render(SDL_Renderer* renderer, const TextureManager& textureManager) const
{
	SDL_Rect rect = {m_EntityDetails.posX, m_EntityDetails.posY, m_EntityBounds.width, m_EntityBounds.height};
	SDL_RenderCopy(renderer, textureManager.GetTexture(m_EntityDetails.textureIndex).texture, NULL, &rect);
}

void Entity::HandleCollisions(const Entity& entity)
{
	std::cout << "Handling Collision between " << m_EntityBounds.width << " and " << entity.m_EntityBounds.width << std::endl;
}