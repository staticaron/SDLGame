#include <iostream>
#include <functional>
#include <charconv>

#include "imgui/imgui.h"

#include "managers/config.h"
#include "managers/AudioManager.h"
#include "Level.h"
#include "Bat.h"
#include "Ball.h"


Level::Level()
	: m_EntityManager(
		{ 2, 1000, glm::vec2( 60, 558 ), glm::vec2( 1.5, 1.5 ) },
		{ 0, 1000, glm::vec2( 300, 100 ), glm::vec2( 1, 1 ) }
	), m_Camera( { 0, 0 }, { 1, 1 })
{
	m_TransitionManager.Init( TransitionState::START );
	m_TransitionManager.StartTransition( [this](){ } );
}

Level::~Level() 
{
	std::cout << "Level destroyed" << std::endl;
}

void Level::Unload()
{
	std::cout << "Unload Level not Implemented" << std::endl;
}

void Level::InitColliders( const TextureManager& textureManager )
{
	m_EntityManager.GetBall().InitColliders( textureManager );
	m_EntityManager.GetBat().InitColliders( textureManager );

	m_EntityManager.GetBall().MakeStatic();
}

void Level::HandleCollisions()
{
	auto entities = m_EntityManager.GetEntities();

	auto& ball = m_EntityManager.GetBall();
	auto& bat = m_EntityManager.GetBat();

	for( int x = 0; x < entities.size(); x++ )
	{
		for( int y = 0; y < entities.size(); y++ )
		{
			if( x == y ) continue;

			auto updateScoreFunc = std::bind( &ScoreManager::AddScore, &m_ScoreManager, std::placeholders::_1 );

			entities[x].get().HandleCollisions( entities[y], updateScoreFunc );

			// If the current entity is ball, then apply either random bounce or boost depending on the boost state of the bat.
			if( entities[x].get().GetType() == BALL && ball.IsTouchingBat() )
			{
				if( !bat.IsBoostActive() ) 
				{	
					bat.DisableExpansion();
					ball.RandomBounce();
				}
				else
				{
					int overlapMultiplier = (ball.GetCenter().x < bat.GetCenter().x ? -1 : 1);
					float tOverlap = overlapMultiplier * ( 1 - ( ball.GetLastOverlap().overlapAmount.x / m_EntityManager.GetBat().GetBoundDetails().GetHalfBounds().x ) );
					m_EntityManager.GetBall().ApplyBoost( tOverlap, bat.GetBoostForceAmount() );
					bat.DisableBoost();
					bat.EnableExpansion();
				}
			}
		}
	}
}

void Level::Update( double deltaTime, const InputManager& inputManager )
{
	if( m_TransitionManager.IsTransitioning() )
	{
		m_TransitionManager.Update( deltaTime );
	}
	else{
		if( m_CurrentTimer >= 0 ) 
		{
			m_CurrentTimer -= deltaTime;

			if( m_CountDown != int( m_CurrentTimer ) )
			{
				if( int( m_CurrentTimer ) != 0 )
					AudioManager::Get().PlaySound( 2, 0 );
				else
					AudioManager::Get().PlaySound(3, 0);
			}
		}
		else if( m_CurrentTimer < 0 )
		{
			m_EntityManager.GetBall().MakeDynamic();
		}

		m_EntityManager.GetBat().Update( deltaTime, inputManager );
		m_EntityManager.GetBall().Update( deltaTime, inputManager );

		if( m_EntityManager.GetBall().IsGrounded() ) {
			AudioManager::Get().PlaySound(1, 0);
			m_IsGameOver = true;
		}
	}

	if( inputManager.m_Escape )
	{
		m_TransitionManager.Init( TransitionState::END );
		m_TransitionManager.StartTransition( std::bind( &Level::ExitLevel, this) );
	}
}

void Level::RenderImGui()
{
	ImGui::Begin( "Level Details" );
	ImGui::BeginDisabled();
	ImGui::InputFloat( "Timer Value", &m_CurrentTimer );
	ImGui::EndDisabled();
	ImGui::End();

	m_Camera.RenderImGui();
	m_EntityManager.GetBall().RenderImGui();
	m_EntityManager.GetBat().RenderImGui();
}

void Level::Render( SDL_Renderer* renderer, const TextureManager& textureManager )
{
	SDL_Rect bgRect = { 0, 0, Config::GetWindowSize().x, Config::GetWindowSize().y };
	SDL_RenderCopy( renderer, textureManager.GetBackgroundTexture( 0 ).GetTexture(), NULL, &bgRect);

	m_EntityManager.GetBat().Render( renderer, textureManager );
	m_EntityManager.GetBall().Render( renderer, textureManager );
}

void Level::RenderUI( SDL_Renderer* renderer, const FontManager& fontManager )
{
	RenderScore( fontManager, renderer );
	RenderTimer( fontManager, renderer );
}

void Level::RenderTransitions( SDL_Renderer* renderer, const TextureManager& textureManager )
{
	m_TransitionManager.RenderTransitions( renderer, textureManager );
}

void Level::RestartLevel()
{
	m_IsGameOver = false;

	m_ScoreManager.ResetScore();

	m_EntityManager.GetBall().ResetDetails();
	m_EntityManager.GetBall().MakeStatic();
	m_EntityManager.GetBat().ResetDetails();

	m_CurrentTimer = m_Timer;
}

void Level::ExitLevel()
{
	m_IsExit = true;
}

void Level::RenderScore( const FontManager& fontManager, SDL_Renderer* renderer )
{
	std::string scoreValue = std::to_string( m_ScoreManager.GetScore() );
	TextureContainer container = fontManager.GetTextureFromFont( renderer, 0, scoreValue.c_str(), SDL_Color{ 200, 200, 200 } );

	SDL_Rect fontRect = { Config::GetWindowSize().x - Config::GetWindowPadding() - container.GetDimensions().x, 0, container.GetDimensions().x, container.GetDimensions().y };

	SDL_RenderCopy( renderer, container.GetTexture(), NULL, &fontRect);

	container.Destroy();
}

void Level::RenderTimer( const FontManager& fontManager, SDL_Renderer* renderer )
{
	if( m_CurrentTimer < 0 ) return;

	m_CountDown = int( m_CurrentTimer );

	std::string timerStr = std::to_string( int( m_CurrentTimer ) );
	TextureContainer container = fontManager.GetTextureFromFont( renderer, 0, timerStr.c_str(), SDL_Color{ 200, 200, 200 } );

	float xScale = 2;
	float yScale = 2;
	SDL_Rect timerRect = { Config::GetWindowSize().x * 0.5f - container.GetDimensions().x * xScale * 0.5f, Config::GetWindowSize().y * 0.5f - container.GetDimensions().y * yScale * 0.5f, container.GetDimensions().x * xScale, container.GetDimensions().y * yScale };

	SDL_RenderCopy( renderer, container.GetTexture(), NULL, &timerRect);

	container.Destroy();
}