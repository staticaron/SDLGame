#pragma once

#include <vector>

#include "SDL.h"

#include "Camera.h"
#include "Entity.h"
#include "levels/ILevel.h"
#include "managers/EntityManager.h"
#include "managers/FontManager.h"
#include "managers/InputManager.h"
#include "managers/ScoreManager.h"
#include "managers/TextureManager.h"
#include "managers/TransitionManager.h"

class Level : public ILevel
{
public:
	Level();
	~Level();

	void Unload();

	void InitColliders( const TextureManager& );

	void HandleCollisions();
	void Update( double, const InputManager& );

	void Render( SDL_Renderer*, const TextureManager& ) override;
	void RenderUI( SDL_Renderer*, const FontManager&, const TextureManager& ) override;
	void RenderTransitions( SDL_Renderer*, const TextureManager& ) override;
	void RenderImGui() override;

	void StartLevel();
	void RestartLevel();
	void ExitLevel();

	bool IsGameOver() const { return m_IsGameOver; }
	bool IsExit() const { return m_IsExit; }

	void RenderScore( const FontManager& fontManager, SDL_Renderer* renderer );
	void RenderTimer( const FontManager& fontManager, SDL_Renderer* renderer );

private:
	EntityManager m_EntityManager;
	ScoreManager m_ScoreManager;
	TransitionManager m_TransitionManager;
	Camera m_Camera;

	float m_Timer = 5.0f;
	float m_CurrentTimer = m_Timer;
	float m_CountDown = -1;

	bool m_IsGameOver = false;
	bool m_IsExit = false;

	int m_TimerYAdditive = -100;

	TransitionState m_TransitionState = TransitionState::START;
	bool m_IsTransitioning = true;
	float m_TransitionTime = 1.0f;
	float m_CurrentTransitionTime = 0.0f;
};