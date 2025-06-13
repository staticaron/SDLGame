#include <iostream>
#include <charconv>

#include "Level.h"
#include "Bat.h"
#include "Ball.h"
#include "managers/config.h"


Level::Level()
: m_EntityManager(
{2, 1, glm::vec2(250, 558), glm::vec2(2, 2)}, 
{0, 0, glm::vec2(300, 100), glm::vec2(2, 2)}
) {}

Level::~Level() {}

void Level::InitColliders(const TextureManager& textureManager)
{
	m_EntityManager.GetBall().InitColliders(textureManager);
	m_EntityManager.GetBat().InitColliders(textureManager);
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

			bool isGrounded = entities[x].get().HandleCollisions(entities[y], updateScoreFunc);

			if(entities[x].get().GetType() == BALL && isGrounded)
			{ 
				m_IsGameOver = true;
				break;
			}
		}
	}
}

void Level::Update(double deltaTime, const InputManager& inputManager)
{
	m_EntityManager.GetBat().Update(deltaTime, inputManager);
	m_EntityManager.GetBall().Update(deltaTime, inputManager);
}

void Level::RenderImGui()
{
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
	// if( m_ScoreManager.GetScore() == m_PreviousScore ){ 
	// TODO : Render Same texture if the font doesn't change.

	std::string scoreValue = std::to_string(m_ScoreManager.GetScore());
	SDL_Surface* fontSurface = TTF_RenderText_Solid(fontManager.GetFont(0), scoreValue.c_str(), SDL_Color{200, 200, 200});

	if(fontSurface == NULL) 
	{
		std::cout << "ERROR Creating FontSurface" << " " << SDL_GetError() << std::endl;
		return; 
	}

	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);

	SDL_FreeSurface( fontSurface );

	if( fontTexture == NULL )
	{
		std::cout << "ERROR Creating FontTexture" << " " << SDL_GetError() << std::endl;
		return;
	}

	int fontWidth = fontSurface->w; 
	int fontHeight = fontSurface->h;

	SDL_Rect fontRect = { Config::GetWindowSize().x - Config::GetWindowPadding() - fontWidth, 0, fontWidth, fontHeight};

	SDL_RenderCopy(renderer, fontTexture, NULL, &fontRect);

	SDL_DestroyTexture( fontTexture );

	m_PreviousScore = m_ScoreManager.GetScore();
}
