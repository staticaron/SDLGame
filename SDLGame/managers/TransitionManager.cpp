#include "TransitionManager.h"

#include "config.h"

void TransitionManager::Init( TransitionState transitionState )
{
	m_TransitionState = transitionState;
}

void TransitionManager::StartTransition( std::function<void()> transitionEndCallback )
{
	m_TransitionEndCallback = transitionEndCallback;
	m_IsTransitioning = true;
	m_CurrentTransitionTime = 0.0f;
}

void TransitionManager::Update( double deltaTime )
{
	if( m_IsTransitioning )
	{
		m_CurrentTransitionTime += deltaTime;

		if( m_CurrentTransitionTime >= m_TransitionTime )
		{
			m_IsTransitioning = false;
			m_TransitionEndCallback();
		}
	}
}

void TransitionManager::RenderTransitions( SDL_Renderer* renderer, const TextureManager& textureManager )
{
	if( m_TransitionState == TransitionState::NONE ) return;

	SDL_Rect transitionRect = { 0, 0, Config::GetWindowSize().x, Config::GetWindowSize().y };
	SDL_Texture* transitionTexture = textureManager.GetBackgroundTexture( 2 ).GetTexture();

	SDL_SetTextureBlendMode( transitionTexture, SDL_BLENDMODE_BLEND );
	Uint8 alphaValue = (m_CurrentTransitionTime / m_TransitionTime) * 255;

	if( m_TransitionState == TransitionState::START ) alphaValue = 255 - alphaValue;

	SDL_SetTextureAlphaMod( transitionTexture, alphaValue );

	SDL_RenderCopy( renderer, transitionTexture, NULL, &transitionRect );
}
