#pragma once

#include "SDL/SDL.h"
#include "Entity.h"

class Ball : public Entity 
{
	public:
	Ball(EntityDetails details);

	void Update(double, const InputManager& inputManager) override;
	void AddForce(float forceX, float forceY);

	private:
	float m_VelocityX, m_VelocityY;

	int paddingY = 10;
};