#include "Ball.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/random.hpp"

#include <iostream>

Ball::Ball( EntityDetails details )
	: Entity( EntityType::BALL, details ), m_VelocityX( 0 ), m_VelocityY( 0 )
{

}

void Ball::Update( double deltaTime, const InputManager& inputManager )
{
	m_PreviousEntityDetails = m_EntityDetails;

	if( inputManager.m_Space ) AddForce( { 0, -1 } );

	// X VELOCITY
	m_EntityDetails.pos.x = m_EntityDetails.pos.x + m_VelocityX * deltaTime;

	// Y VELOCITY
	m_VelocityY = m_VelocityY + 0.002 * deltaTime;
	m_EntityDetails.pos.y = m_EntityDetails.pos.y + m_VelocityY * deltaTime;

	MaintainBounds();

	m_PreviousCollisionAndOverlap = m_CollisionAndOverlap;
}

void Ball::ResolveCollision( const Entity& entity )
{
	if( !m_CollisionAndOverlap.IsColliding() ) return;

	m_EntityDetails.pos.y -= m_CollisionAndOverlap.overlapAmount.y;
	m_VelocityY = 0;

	if( m_CollisionAndOverlap.IsColliding() )
	{
		float randomRotation = glm::linearRand( -60, 60 );

		glm::mat4 rotationMtx = glm::mat4( 1.0f );
		rotationMtx = glm::rotate( rotationMtx, glm::radians( randomRotation ), glm::vec3( 0, 0, 1 ) );

		glm::vec4 forceVector = rotationMtx * glm::vec4( glm::vec2{ 0, -1 }, 1, 1 );

		AddForce( { forceVector.x, forceVector.y } );
	}
}

void Ball::MaintainBounds()
{
	if( m_EntityDetails.pos.y + m_EntityBounds.bounds.y > 600 - paddingY )
	{
		m_EntityDetails.pos.y = 600 - paddingY - m_EntityBounds.bounds.y;
		m_VelocityY = 0;
	}

	if( m_EntityDetails.pos.x + m_EntityBounds.bounds.x > 800 - paddingY )
	{
		m_EntityDetails.pos.x = 800 - paddingY - m_EntityBounds.bounds.x;
		m_VelocityX = 0;
	}
	else if( m_EntityDetails.pos.x < paddingY )
	{
		m_EntityDetails.pos.x = paddingY;
		m_VelocityX = 0;
	}
}

void Ball::AddForce( glm::vec2 force )
{
	m_VelocityX = m_VelocityX + force.x;
	m_VelocityY = m_VelocityY + force.y;
}
