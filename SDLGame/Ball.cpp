#include "Ball.h"

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/random.hpp"
#include "imgui/imgui.h"

#include "managers/config.h"


Ball::Ball( EntityDetails details )
	: Entity( EntityType::BALL, details ), m_VelocityX( 0 ), m_VelocityY( 0 )
{

}

void Ball::Update( double deltaTime, const InputManager& inputManager )
{
	if ( m_IsStatic ) return;

	GroundCheck();

	m_PreviousEntityDetails = m_EntityDetails;

	if( inputManager.m_Space ) AddForce( { 0, -2 } );

	m_EntityDetails.pos.x = m_EntityDetails.pos.x + m_VelocityX * deltaTime * m_EntityDetails.moveSpeed;

	m_VelocityY = m_VelocityY + 2 * deltaTime;
	m_EntityDetails.pos.y = m_EntityDetails.pos.y + m_VelocityY * deltaTime * m_EntityDetails.moveSpeed;

	MaintainBounds();

	m_PreviousCollisionAndOverlap = m_CollisionAndOverlap;
}

void Ball::ResolveCollision( const Entity& entity )
{
	if( !m_CollisionAndOverlap.IsColliding() ) return;

	m_EntityDetails.pos.y -= m_CollisionAndOverlap.overlapAmount.y;
	m_VelocityY = 0;

	int minRange = -60;
	int maxRange = 60;

	if( m_CollisionAndOverlap.IsColliding() )
	{
		// If touching the walls, push the ball slightly to the opposite direction.
		if( GetBoundPoint(BOTTOMLEFT).x <= Config::GetWindowPadding() )
		{
			minRange = 0;
			maxRange = 30;
		}
		else if ( GetBoundPoint(BOTTOMRIGHT).x >= Config::GetWindowSize().x - Config::GetWindowPadding() )
		{
			minRange = -30;
			maxRange = 0;
		}

		float randomRotation = glm::linearRand( minRange, maxRange );

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

void Ball::ResetDetails()
{
	m_EntityDetails = m_InitialDetails;
	m_IsGrounded = false;
	m_VelocityX = 0;
	m_VelocityY = 0;
}

void Ball::RenderImGui()
{
	Entity::RenderImGui();

	std::string windowTitle = m_Type == BALL ? "Ball" : "Bat";
	windowTitle += " Settings";

	float velocity[2] = { m_VelocityX, m_VelocityY };

	ImGui::Begin(windowTitle.c_str());
	ImGui::BeginDisabled();
	ImGui::InputFloat2("Velocity", velocity);
	ImGui::EndDisabled();
	ImGui::End();
}

void Ball::AddForce( glm::vec2 force )
{
	m_VelocityX = m_VelocityX + force.x;
	m_VelocityY = m_VelocityY + force.y;
}


