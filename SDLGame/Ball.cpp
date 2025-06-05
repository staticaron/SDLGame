#include "Ball.h"

#include <iostream>

Ball::Ball(EntityDetails details) 
: Entity(EntityType::BALL, details), m_VelocityX(0), m_VelocityY(0)
{

}

void Ball::Update(double deltaTime, const InputManager& inputManager)
{
	m_PreviousEntityDetails = m_EntityDetails;

	if (inputManager.m_Space) {
		AddForce(0, -1);
	}

	m_VelocityY = m_VelocityY + 0.001 * deltaTime;
	m_EntityDetails.pos.y = m_EntityDetails.pos.y + m_VelocityY * deltaTime;

	if(m_EntityDetails.pos.y + m_EntityBounds.height > 600 - paddingY){
		m_EntityDetails.pos.y = 600 - paddingY - m_EntityBounds.height;
		m_VelocityY = 0;
	}
}

void Ball::ResolveCollision(const Entity& entity)
{
	AddForce(0, -1);
}

void Ball::AddForce(float forceX, float forceY)
{
	m_VelocityX = m_VelocityX + forceX;
	m_VelocityY = m_VelocityY + forceY;
}
