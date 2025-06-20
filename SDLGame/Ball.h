#pragma once

#include "glm/vec2.hpp"

#include "Entity.h"
#include "SDL/SDL.h"

enum class BallState
{
	BOUNCE, BOOST, NONE
};

class Ball : public Entity
{
public:
	Ball( EntityDetails details );

	void Update( double, const InputManager& inputManager ) override;

	AxisOverlap DetectCollision( const Entity& ) override;
	void ResolveCollision( const Entity& ) override;
	void MaintainBounds() override;

	void ResetDetails() override;
	void RenderImGui() override;

	void RandomBounce();
	void ApplyBoost( float, float );

	void AddForce( glm::vec2 force );

	AxisOverlap GetLastOverlap() const { return m_LastAxisOverlap; };
	bool IsTouchingBat() const { return m_IsTouchingBat; };

private:
	float m_VelocityX, m_VelocityY;
	bool m_IsTouchingBat = false;

	float m_MinBounceAngle = -50;
	float m_MaxBounceAngle = 50;
	float m_MinBoostAngle = -40;
	float m_MaxBoostAngle = 40;

	BallState m_CurrentBallState = BallState::BOUNCE;
	AxisOverlap m_LastAxisOverlap;
};