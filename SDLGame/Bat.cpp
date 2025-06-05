#include "Bat.h"

#include <iostream>

Bat::Bat(EntityDetails details) 
: Entity(EntityType::BAT, details)
{
}

void Bat::Update(double deltaTime, const InputManager& inputManager)
{
	m_PreviousEntityDetails = m_EntityDetails;

	m_EntityDetails.pos.x += inputManager.m_HorizontalAxis * m_EntityDetails.moveSpeed * deltaTime;

	MaintainBounds();
}

void Bat::MaintainBounds()
{
	if (m_EntityDetails.pos.x < paddingX) m_EntityDetails.pos.x = paddingX;
	if (m_EntityDetails.pos.x + m_EntityBounds.width > 800 - paddingX) m_EntityDetails.pos.x = 800 - paddingX - m_EntityBounds.width;
}
