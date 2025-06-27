#pragma once

#include <functional>
#include <vector>

#include "../Ball.h"
#include "../Bat.h"
#include "../Diamond.h"

class EntityManager
{
public:
	EntityManager( EntityDetails, EntityDetails, EntityDetails );

	Bat& GetBat() { return m_Bat; };
	Ball& GetBall() { return m_Ball; };
	Diamond& GetDiamond() { return m_Diamond; };

	std::vector<std::reference_wrapper<Entity>> GetEntities();

private:
	Bat m_Bat;
	Ball m_Ball;
	Diamond m_Diamond;
};