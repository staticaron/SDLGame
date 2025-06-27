#pragma once

#include <array>
#include <functional>
#include <string>

#include "glm.hpp"
#include "SDL.h"

#include "managers/InputManager.h"
#include "managers/TextureManager.h"

struct EntityDetails
{
	int textureIndex = 0;
	float moveSpeed = 1;
	glm::vec2 pos = glm::vec2( 0, 0 );
	glm::vec2 scale = glm::vec2( -1, -1 );
};

struct EntityBounds
{
	glm::vec2 bounds;

	EntityBounds() : bounds( { -1, -1 } ) {}

	EntityBounds( const TextureManager& textureManager, const EntityDetails& details )
	{
		const TextureContainer& texContainer = textureManager.GetTexture( details.textureIndex );

		bounds.x = texContainer.GetDimensions().x * details.scale.x;
		bounds.y = texContainer.GetDimensions().y * details.scale.y;
	}

	glm::vec2 GetHalfBounds() const { return { bounds.x * 0.5f, bounds.y * 0.5f }; }
};

struct AxisOverlap
{
	bool xOverlap, yOverlap = false;
	glm::vec2 overlapAmount = { -1, -1 };

	bool IsColliding() { return xOverlap && yOverlap; }
};

enum BoundPointType
{
	TOPLEFT,
	TOPRIGHT,
	BOTTOMRIGHT,
	BOTTOMLEFT
};

enum EntityType
{
	BALL,
	BAT,
	DIAMOND
};

class Entity
{
public:
	Entity( EntityType type, EntityDetails details );
	~Entity();

	virtual void InitColliders( const TextureManager& );

	virtual void Update( double, const InputManager& );
	virtual void HandleCollisions( const Entity&, std::function<void( int )> );
	virtual void Render( SDL_Renderer* renderer, const TextureManager& ) const;
	virtual void RenderImGui();

	virtual AxisOverlap DetectCollision( const Entity& entity );
	virtual void ResolveCollision( const Entity& );
	virtual void MaintainBounds();

	virtual void ResetDetails();
	virtual void GroundCheck();

	// Getters and Setters
	std::array<glm::vec2, 4> GetBoundPoints() const;
	glm::vec2 GetBoundPoint( BoundPointType type ) const;
	glm::vec2 GetCenter() const;
	EntityDetails GetEntityDetails() const { return m_EntityDetails; };
	EntityBounds GetBoundDetails() const { return m_EntityBounds; };
	EntityType GetType() const { return m_Type; }
	bool IsGrounded() const { return m_IsGrounded; }

	void MakeStatic() { m_IsStatic = true; }
	void MakeDynamic() { m_IsStatic = false; }

	void SetBoundPoint( BoundPointType, glm::vec2 );

protected:
	EntityType m_Type;
	bool m_ShowBounds = true;
	bool m_IsGrounded = false;

	bool m_IsStatic = false;

	EntityDetails m_DefaultDetails;
	EntityBounds m_DefaultBounds;

	AxisOverlap m_CollisionAndOverlap;
	AxisOverlap m_PreviousCollisionAndOverlap;

	EntityDetails m_EntityDetails;
	EntityDetails m_PreviousEntityDetails;
	EntityBounds m_EntityBounds;
};
