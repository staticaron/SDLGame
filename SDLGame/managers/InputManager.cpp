#include "InputManager.h"

#include <iostream>
#include <algorithm>

int getHorizotnalAxis() 
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	int leftBtn = keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT];
	int rightBtn = keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT];

	if(leftBtn && !rightBtn) return -1;
	if(!leftBtn && rightBtn) return 1;
	
	return 0;
}

int getVerticalAxis()
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	int upBtn = keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP];
	int downBtn = keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN];

	if (upBtn && !downBtn) return -1;
	if (!upBtn && downBtn) return 1;

	return 0;
}

void InputManager::InitProcessSession()
{
	m_Space = false;
}

void InputManager::ProcessEvent(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN && !event.key.repeat)
	{
		if(event.key.keysym.sym == SDLK_SPACE) m_Space = true;
	}
	else if (event.type == SDL_KEYUP && !event.key.repeat)
	{
		if (event.key.keysym.sym == SDLK_SPACE) m_Space = false;
	}

	m_HorizontalAxis = getHorizotnalAxis();
	m_VerticalAxis = getVerticalAxis();
}
