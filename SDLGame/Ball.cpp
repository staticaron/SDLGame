#include "Ball.h"

#include <iostream>

Ball::Ball(EntityDetails details) 
: Entity(details), m_VelocityX(0), m_VelocityY(0)
{

}

void Ball::Update(double deltaTime, const InputManager& inputManager)
{
	if (inputManager.m_Space) {
		AddForce(0, -1);
	}

	m_VelocityY = m_VelocityY + 0.001 * deltaTime;
	m_EntityDetails.posY = m_EntityDetails.posY + m_VelocityY * deltaTime;

	if(m_EntityDetails.posY + m_EntityBounds.height > 600 - paddingY){
		m_EntityDetails.posY = 600 - paddingY - m_EntityBounds.height;
		m_VelocityY = 0;
	}
}

void Ball::AddForce(float forceX, float forceY)
{
	m_VelocityX = m_VelocityX + forceX;
	m_VelocityY = m_VelocityY + forceY;
}
