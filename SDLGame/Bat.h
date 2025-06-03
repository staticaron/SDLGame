#pragma once

#include "SDL/SDL.h"
#include "Entity.h"

class Bat : public Entity 
{
	public:
	Bat(EntityDetails details);

	void Update(double, const InputManager& inputManager) override;
	private:
	int paddingX = 10;

	void MaintainBounds();
};