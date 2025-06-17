#pragma once

#include <SDL/SDL.h>
#include <vector>

#include "Entity.h"
#include "Camera.h"
#include "managers/TextureManager.h"
#include "managers/FontManager.h"
#include "managers/InputManager.h"
#include "managers/EntityManager.h"
#include "managers/ScoreManager.h"

enum class TransitionState
{
	START, END, NONE
};

class Level
{
public:
	Level();
	~Level();

	void Unload();

	void InitColliders( const TextureManager& );

	void HandleCollisions();
	void Update( double, const InputManager& );

	void Render( SDL_Renderer*, const TextureManager& );
	void RenderUI( SDL_Renderer*, const FontManager& );
	void RenderTransitions( SDL_Renderer*, const TextureManager& ); 
	void RenderImGui();

	void RestartLevel();

	bool IsGameOver() const { return m_IsGameOver; }
	bool IsExit() const { return m_IsExit; }

	void ChangeTransitionState( TransitionState );

	void RenderScore( const FontManager& fontManager, SDL_Renderer* renderer );
	void RenderTimer( const FontManager& fontManager, SDL_Renderer* renderer );

private:
	EntityManager m_EntityManager;
	ScoreManager m_ScoreManager;
	Camera m_Camera;

	float m_Timer = 5.0f;
	float m_CurrentTimer = m_Timer;
	float m_CountDown = -1;

	bool m_IsGameOver = false;
	bool m_IsExit = false;

	TransitionState m_TransitionState = TransitionState::START;
	bool m_IsTransitioning = true;
	float m_TransitionTime = 1.0f;
	float m_CurrentTransitionTime = 0.0f;
};