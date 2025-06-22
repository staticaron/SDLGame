#pragma once

#include <functional>

#include "SDL/SDL.h"

#include "TextureManager.h"

enum class TransitionState
{
	START, END, NONE
};

class TransitionManager
{
	public:
	void Init( TransitionState );
	void StartTransition( std::function<void()> transitionEndCallback  );
	void Update( double );
	void RenderTransitions( SDL_Renderer*, const TextureManager& );

	bool IsTransitioning() const { return m_IsTransitioning; };

	private:
	TransitionState m_TransitionState = TransitionState::NONE;
	float m_TransitionTime = .5f;
	float m_CurrentTransitionTime = 0.0f;
	bool m_IsTransitioning = true;

	std::function<void()> m_TransitionEndCallback;
};