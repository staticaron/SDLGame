#pragma once

#include <SDL/SDL.h>
#include <string>

#include "managers/TextureManager.h"
#include "managers/InputManager.h"

struct EntityDetails {
	int textureIndex = 0;
	float moveSpeed = 1;
	float posX, posY = 0;
	float scaleX, scaleY = -1;
};

struct EntityBounds {
	int width = -1;
	int height = -1;

	EntityBounds() {}

	EntityBounds(const TextureManager& textureManager, const EntityDetails& details) 
	{
		const TextureContainer& texContainer = textureManager.GetTexture(details.textureIndex);

		width = texContainer.width * details.scaleX;
		height = texContainer.height * details.scaleY;
	}
};

class Entity 
{
	public:
	Entity(EntityDetails details);
	~Entity();

	virtual void InitColliders(const TextureManager&);

	virtual void Update(double, const InputManager&);
	virtual void Render(SDL_Renderer* renderer, const TextureManager&) const;
	virtual void HandleCollisions(const Entity&);


	protected:
	EntityDetails m_EntityDetails;
	EntityDetails m_PreviousEntityDetails;
	EntityBounds m_EntityBounds = EntityBounds();
};
