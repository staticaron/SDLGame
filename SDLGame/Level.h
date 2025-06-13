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

	void InitColliders(const TextureManager&);

	void UpdateScore(int deltaScore);

	void HandleCollisions();
	void Update(double, const InputManager&);
	void RenderImGui();
	void Render(SDL_Renderer*, const TextureManager&);

	void RenderUI(SDL_Renderer*, const FontManager&);

	bool IsGameOver(){ return m_IsGameOver; }

	private:
	EntityManager m_EntityManager;
	ScoreManager m_ScoreManager;

	int m_PreviousScore = -1;

	bool m_IsGameOver = false;
};