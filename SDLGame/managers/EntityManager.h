#pragma once

#include <vector>
#include <functional>

#include "../Bat.h"
#include "../Ball.h"

class EntityManager 
{
	public:
	EntityManager(EntityDetails batDetails, EntityDetails ballDetails);

	Bat& GetBat(){ return m_Bat; };
	Ball& GetBall(){ return m_Ball; };

	std::vector<std::reference_wrapper<Entity>> GetEntities();

	private:
	Bat m_Bat;
	Ball m_Ball;
};