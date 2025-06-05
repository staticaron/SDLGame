#pragma once

#include <SDL/SDL.h>
#include <string>
#include <array>

#include "managers/TextureManager.h"
#include "managers/InputManager.h"

struct Vector2
{
	float x = 0;
	float y = 0;

	Vector2(float xParam, float yParam)
	{
		x = xParam; y = yParam;
	}
};

struct EntityDetails {
	int textureIndex = 0;
	float moveSpeed = 1;
	Vector2 pos = Vector2(0, 0);
	Vector2 scale = Vector2(-1, -1);
};

struct EntityBounds {
	int width = -1;
	int height = -1;

	EntityBounds() {}

	EntityBounds(const TextureManager& textureManager, const EntityDetails& details) 
	{
		const TextureContainer& texContainer = textureManager.GetTexture(details.textureIndex);

		width = texContainer.width * details.scale.x;
		height = texContainer.height * details.scale.y;
	}
};

struct AxisOverlap
{
	bool isXOverlap = false;
	bool isYOverlap = false;
	float xOverlap = 0;
	float yOverlap = 0;
};

enum BoundPointType {
	TOPLEFT,
	TOPRIGHT,
	BOTTOMRIGHT,
	BOTTOMLEFT
};

enum EntityType
{
	BALL,
	BAT
};

class Entity 
{
	public:
	Entity(EntityType type, EntityDetails details);
	~Entity();

	virtual void InitColliders(const TextureManager&);

	virtual void Update(double, const InputManager&);
	virtual void Render(SDL_Renderer* renderer, const TextureManager&) const;

	bool DetectCollision(const Entity& entity) const;
	virtual void ResolveCollision(const Entity&);
	virtual void HandleCollisions(const Entity&);

	std::array<Vector2, 4> GetBoundPoints() const;
	Vector2 GetBoundPoint(BoundPointType type) const;

	EntityType GetType() const { return m_Type; }

	protected:
	EntityType m_Type;

	EntityDetails m_EntityDetails;
	EntityDetails m_PreviousEntityDetails;
	EntityBounds m_EntityBounds = EntityBounds();
};
