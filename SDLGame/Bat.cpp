#include "Bat.h"

#include <iostream>

Bat::Bat(EntityDetails details) 
: Entity(details)
{
}

void Bat::Update(double deltaTime, const InputManager& inputManager)
{
	m_EntityDetails.posX += inputManager.m_HorizontalAxis * m_EntityDetails.moveSpeed * deltaTime;

	MaintainBounds();
}

void Bat::MaintainBounds()
{
	if (m_EntityDetails.posX < paddingX) m_EntityDetails.posX = paddingX;
	if (m_EntityDetails.posX + m_EntityBounds.width > 800 - paddingX) m_EntityDetails.posX = 800 - paddingX - m_EntityBounds.width;
}
