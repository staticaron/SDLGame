#include <iostream>

#include "Entity.h"
#include "SDL/SDL_image.h"

Entity::Entity(EntityType type, EntityDetails details) 
: m_EntityDetails(details), m_Type(type)
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
	SDL_Rect rect = {m_EntityDetails.pos.x, m_EntityDetails.pos.y, m_EntityBounds.width, m_EntityBounds.height};
	SDL_RenderCopy(renderer, textureManager.GetTexture(m_EntityDetails.textureIndex).texture, NULL, &rect);
}

bool Entity::DetectCollision(const Entity& entity) const
{
	for (auto& x : GetBoundPoints()) 
	{
		bool isXOverlap = x.x >= entity.m_EntityDetails.pos.x && x.x <= entity.m_EntityDetails.pos.x + entity.m_EntityBounds.width;
		bool isYOverlap = x.y >= entity.m_EntityDetails.pos.y && x.y <= entity.m_EntityDetails.pos.y + entity.m_EntityBounds.height;

		if(isXOverlap && isYOverlap) return true;
	}

	return false;
}

void Entity::ResolveCollision(const Entity& entity)
{
	std::cout << "Collision Resolution Not Implemented!" << std::endl;
}

void Entity::HandleCollisions(const Entity& entity)
{
	if (DetectCollision(entity) && m_Type == BALL)
	{
		ResolveCollision(entity);
	}
}

std::array<Vector2, 4> Entity::GetBoundPoints() const
{
	return 
	{
		Vector2(m_EntityDetails.pos.x, m_EntityDetails.pos.y), 
		Vector2(m_EntityDetails.pos.x + m_EntityBounds.width, m_EntityDetails.pos.y),
		Vector2(m_EntityDetails.pos.x + m_EntityBounds.width, m_EntityDetails.pos.y + m_EntityBounds.height),
		Vector2(m_EntityDetails.pos.x, m_EntityDetails.pos.y + m_EntityBounds.height)
	};
}

Vector2 Entity::GetBoundPoint(BoundPointType type) const
{
	return Vector2(m_EntityDetails.pos.x + (type == TOPRIGHT || type == BOTTOMRIGHT ? m_EntityBounds.width : 0), m_EntityDetails.pos.y + (type == BOTTOMRIGHT || type == BOTTOMLEFT ? m_EntityBounds.height : 0));
}
