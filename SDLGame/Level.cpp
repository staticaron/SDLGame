#include <iostream>
#include <charconv>

#include "imgui/imgui.h"

#include "Level.h"
#include "Bat.h"
#include "Ball.h"
#include "managers/config.h"


Level::Level()
: m_EntityManager(
{2, 1000, glm::vec2(250, 558), glm::vec2(2, 2)}, 
{0, 1000, glm::vec2(300, 100), glm::vec2(2, 2)}
) {}

Level::~Level() {}

void Level::InitColliders(const TextureManager& textureManager)
{
	m_EntityManager.GetBall().InitColliders(textureManager);
	m_EntityManager.GetBat().InitColliders(textureManager);

	// Make static while the timer is running.
	m_EntityManager.GetBall().MakeStatic();
}

void Level::HandleCollisions() 
{
	auto entities = m_EntityManager.GetEntities();

	for(int x = 0; x < entities.size(); x++)
	{
		for (int y = 0; y < entities.size(); y++) 
		{
			if(x == y) continue;

			auto updateScoreFunc = std::bind(&ScoreManager::AddScore, &m_ScoreManager, std::placeholders::_1 );

			entities[x].get().HandleCollisions(entities[y], updateScoreFunc);

			if(entities[x].get().GetType() == BALL && entities[x].get().IsGrounded() )
			{ 
				m_IsGameOver = true;
				break;
			}
		}
	}
}

bool Level::Update(double deltaTime, const InputManager& inputManager)
{
	if( m_CurrentTimer >= 0 ) m_CurrentTimer -= deltaTime;
	else if( m_CurrentTimer < 0 )
	{
		m_EntityManager.GetBall().MakeDynamic();
	}

	m_EntityManager.GetBat().Update(deltaTime, inputManager);
	m_EntityManager.GetBall().Update(deltaTime, inputManager);

	if( m_EntityManager.GetBall().IsGrounded() ) return false;
	return true;
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

void Level::Render(SDL_Renderer* renderer, const TextureManager& textureManager)
{
	m_EntityManager.GetBat().Render(renderer, textureManager);
	m_EntityManager.GetBall().Render(renderer, textureManager);
}

void Level::RenderUI(SDL_Renderer* renderer, const FontManager& fontManager)
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
	SDL_Surface* fontSurface = TTF_RenderText_Solid( fontManager.GetFont( 0 ), scoreValue.c_str(), SDL_Color{ 200, 200, 200 } );

	if( fontSurface == NULL )
	{
		std::cout << "ERROR Creating FontSurface" << " " << SDL_GetError() << std::endl;
		return;
	}

	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface( renderer, fontSurface );

	int fontWidth = fontSurface->w;
	int fontHeight = fontSurface->h;

	SDL_FreeSurface( fontSurface );

	if( fontTexture == NULL )
	{
		std::cout << "ERROR Creating FontTexture" << " " << SDL_GetError() << std::endl;
		return;
	}

	SDL_Rect fontRect = { Config::GetWindowSize().x - Config::GetWindowPadding() - fontWidth, 0, fontWidth, fontHeight };

	SDL_RenderCopy( renderer, fontTexture, NULL, &fontRect );

	SDL_DestroyTexture( fontTexture );
}

void Level::RenderTimer( const FontManager& fontManager, SDL_Renderer* renderer )
{
	if ( m_CurrentTimer < 0 ) return;

	std::string timerStr = std::to_string(int(m_CurrentTimer));
	SDL_Surface* timerSurface = TTF_RenderText_Solid( fontManager.GetFont(0),  timerStr.c_str(), SDL_Color{ 200, 200, 200 } );

	if( timerSurface == NULL )
	{
		std::cout << "ERROR Creating FontSurface" << " " << SDL_GetError() << std::endl;
		return;
	}

	SDL_Texture* timerTexture = SDL_CreateTextureFromSurface( renderer, timerSurface );

	int fontWidth = timerSurface->w * 2;
	int fontHeight = timerSurface->h * 2;

	SDL_FreeSurface( timerSurface );

	if( timerTexture == NULL )
	{
		std::cout << "ERROR Creating FontTexture" << " " << SDL_GetError() << std::endl;
		return;
	}

	SDL_Rect timerRect = { Config::GetWindowSize().x * 0.5f - fontWidth * 0.5f, Config::GetWindowSize().y * 0.5f - fontHeight * 0.5f, fontWidth, fontHeight };

	SDL_RenderCopy( renderer, timerTexture, NULL, &timerRect);

	SDL_DestroyTexture( timerTexture );
}