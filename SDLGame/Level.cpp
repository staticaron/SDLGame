#include <iostream>
#include <charconv>

#include "imgui/imgui.h"

#include "managers/config.h"
#include "managers/AudioManager.h"
#include "Level.h"
#include "Bat.h"
#include "Ball.h"


Level::Level()
	: m_EntityManager(
		{ 2, 1000, glm::vec2( 250, 558 ), glm::vec2( 2, 2 ) },
		{ 0, 1000, glm::vec2( 300, 100 ), glm::vec2( 2, 2 ) }
	)
{}

Level::~Level() {}

void Level::Unload()
{
	std::cout << "Unload Level not Implemented" << std::endl;
}

void Level::InitColliders( const TextureManager& textureManager )
{
	m_EntityManager.GetBall().InitColliders( textureManager );
	m_EntityManager.GetBat().InitColliders( textureManager );

	// Make static while the timer is running.
	m_EntityManager.GetBall().MakeStatic();
}

void Level::HandleCollisions()
{
	auto entities = m_EntityManager.GetEntities();

	for( int x = 0; x < entities.size(); x++ )
	{
		for( int y = 0; y < entities.size(); y++ )
		{
			if( x == y ) continue;

			auto updateScoreFunc = std::bind( &ScoreManager::AddScore, &m_ScoreManager, std::placeholders::_1 );

			entities[x].get().HandleCollisions( entities[y], updateScoreFunc );
		}
	}
}

void Level::Update( double deltaTime, const InputManager& inputManager )
{
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

void Level::RenderImGui()
{
	ImGui::Begin( "Level Details" );
	ImGui::BeginDisabled();
	ImGui::InputFloat( "Timer Value", &m_CurrentTimer );
	ImGui::EndDisabled();
	ImGui::End();

	m_EntityManager.GetBall().RenderImGui();
	m_EntityManager.GetBat().RenderImGui();
}

void Level::Render( SDL_Renderer* renderer, const TextureManager& textureManager )
{
	// Rendering Background
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

void Level::RestartLevel()
{
	m_IsGameOver = false;

	m_ScoreManager.ResetScore();

	m_EntityManager.GetBall().ResetDetails();
	m_EntityManager.GetBall().MakeStatic();
	m_EntityManager.GetBat().ResetDetails();

	m_CurrentTimer = m_Timer;
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