#include "Ball.h"

#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/random.hpp"
#include "imgui/imgui.h"

#include "managers/config.h"
#include "managers/AudioManager.h"


Ball::Ball( EntityDetails details )
	: Entity( EntityType::BALL, details ), m_VelocityX( 0 ), m_VelocityY( 0 )
{

}

void Ball::Update( double deltaTime, const InputManager& inputManager )
{
	if ( m_IsStatic ) return;

	GroundCheck();

	m_PreviousEntityDetails = m_EntityDetails;

	m_EntityDetails.pos.x = m_EntityDetails.pos.x + m_VelocityX * deltaTime * m_EntityDetails.moveSpeed;

	m_VelocityY = m_VelocityY + 2 * deltaTime;
	m_EntityDetails.pos.y = m_EntityDetails.pos.y + m_VelocityY * deltaTime * m_EntityDetails.moveSpeed;

	MaintainBounds();

	m_PreviousCollisionAndOverlap = m_CollisionAndOverlap;
}

AxisOverlap Ball::DetectCollision( const Entity& entity )
{
	m_IsTouchingBat = false; 

	auto aO = Entity::DetectCollision( entity ); // Handle the base collision
	if ( aO.IsColliding() && entity.GetType() == BAT ) 
		m_IsTouchingBat = true;

	m_LastAxisOverlap = aO;

	return aO;
}

void Ball::ResolveCollision( const Entity& entity )
{
	if( entity.GetType() == DIAMOND ) return; 

	if( !m_CollisionAndOverlap.IsColliding() ) return;

	m_EntityDetails.pos.y -= m_CollisionAndOverlap.overlapAmount.y;
	m_VelocityY = 0;
}

void Ball::MaintainBounds()
{
	if( GetBoundPoint(BOTTOMRIGHT).y > Config::GetWindowSize().y - Config::GetWindowPadding() )
	{
		SetBoundPoint( BOTTOMRIGHT, { GetBoundPoint(BOTTOMRIGHT).x, Config::GetWindowSize().y - Config::GetWindowPadding() });
		m_VelocityY = 0;
	}
	else if( GetBoundPoint(TOPLEFT).y < Config::GetWindowPadding() )
	{
		SetBoundPoint( TOPLEFT, { GetBoundPoint(TOPLEFT).x, Config::GetWindowPadding() });
		if( m_CurrentBallState == BallState::BOOST ) m_VelocityY = -m_VelocityY * m_BoostWallCollisionDampening;
		AudioManager::Get().PlaySound(5, 0);
	}

	if( GetBoundPoint(BOTTOMRIGHT).x > Config::GetWindowSize().x - Config::GetWindowPadding() )
	{
		SetBoundPoint( BOTTOMRIGHT, { Config::GetWindowSize().x - Config::GetWindowPadding(), GetBoundPoint(BOTTOMRIGHT).y});
		if( m_CurrentBallState == BallState::BOOST ) m_VelocityX = -m_VelocityX * m_BoostWallCollisionDampening;
		else if ( m_CurrentBallState == BallState::BOUNCE ) m_VelocityX = -m_VelocityX * m_BounceWallCollisionDampening;
		AudioManager::Get().PlaySound(5, 0);
	}
	else if( GetBoundPoint(TOPLEFT).x < Config::GetWindowPadding() )
	{
		SetBoundPoint( TOPLEFT, { Config::GetWindowPadding(), GetBoundPoint(TOPLEFT).y } );
		if( m_CurrentBallState == BallState::BOOST ) m_VelocityX = -m_VelocityX * m_BoostWallCollisionDampening;
		else if( m_CurrentBallState == BallState::BOUNCE ) m_VelocityX = -m_VelocityX * m_BounceWallCollisionDampening;
		AudioManager::Get().PlaySound(5, 0);
	}
}

void Ball::ResetDetails()
{
	m_EntityDetails = m_DefaultDetails;
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
	ImGui::InputFloat2( "Velocity", velocity );
	ImGui::Checkbox( "Is Touching Bat", &m_IsTouchingBat );
	ImGui::EndDisabled();
	ImGui::End();
}

void Ball::RandomBounce()
{
	m_VelocityY = 0;
	m_VelocityX = 0;
	m_CurrentBallState = BallState::BOUNCE;

	int minRange = m_MinBounceAngle;
	int maxRange = m_MaxBounceAngle;

	if( m_CollisionAndOverlap.IsColliding() )
	{
		// If touching the walls, push the ball slightly to the opposite direction.
		if( GetBoundPoint( BOTTOMLEFT ).x <= Config::GetWindowPadding() )
		{
			minRange = 0;
			maxRange = 30;
		}
		else if( GetBoundPoint( BOTTOMRIGHT ).x >= Config::GetWindowSize().x - Config::GetWindowPadding() )
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

void Ball::ApplyBoost( float tOverlapAmount, float boostForce )
{
	m_CurrentBallState = BallState::BOOST;

	tOverlapAmount = tOverlapAmount * 0.5 + 0.5;
	float rotation = glm::mix( m_MinBoostAngle, m_MaxBoostAngle, tOverlapAmount );

	glm::mat4 rotationMtx = glm::mat4( 1.0f );
	rotationMtx = glm::rotate( rotationMtx, glm::radians( rotation ), glm::vec3( 0, 0, 1 ) );

	glm::vec4 forceVector = rotationMtx * glm::vec4( glm::vec2{ 0, -1 }, 1, 1 );

	AddForce( forceVector * boostForce );
}

void Ball::AddForce( glm::vec2 force )
{
	m_VelocityX = m_VelocityX + force.x;
	m_VelocityY = m_VelocityY + force.y;
}


