#pragma once

#include "glm/vec2.hpp"

#include "Entity.h"
#include "SDL/SDL.h"

class Ball : public Entity
{
public:
	Ball( EntityDetails details );

	void Update( double, const InputManager& inputManager ) override;
	void ResolveCollision( const Entity& ) override;
	void MaintainBounds() override;

	void AddForce( glm::vec2 force );

private:
	float m_VelocityX, m_VelocityY;

	int paddingY = 10;
};