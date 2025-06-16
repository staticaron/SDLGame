#pragma once

#include <SDL/SDL.h>
#include <vector>

#include "Entity.h"
#include "managers/TextureManager.h"
#include "managers/FontManager.h"
#include "managers/InputManager.h"
#include "managers/EntityManager.h"
#include "managers/ScoreManager.h"

class Level 
{
	public:
	Level();
	~Level();

	void Unload();

	void InitColliders(const TextureManager&);

	void HandleCollisions();
	void Update(double, const InputManager&);
	void RenderImGui();
	void Render(SDL_Renderer*, const TextureManager&);

	void RenderUI(SDL_Renderer*, const FontManager&);
	void RestartLevel();

	bool IsGameOver(){ return m_IsGameOver; }

	private:
	EntityManager m_EntityManager;
	ScoreManager m_ScoreManager;

	float m_Timer = 5.0f;
	float m_CurrentTimer = m_Timer;
	float m_CountDown = -1;

	bool m_IsGameOver = false;

	void RenderScore( const FontManager& fontManager, SDL_Renderer* renderer );
	void RenderTimer( const FontManager& fontManager, SDL_Renderer* renderer );
};