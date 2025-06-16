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
	m_Primary = false;
}

void InputManager::ProcessEvent(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN )
	{
		if(event.key.keysym.sym == SDLK_SPACE) m_Space = true;
	}
	else if (event.type == SDL_KEYUP )
	{
		if (event.key.keysym.sym == SDLK_SPACE) m_Space = false;
	}

	if( event.type == SDL_MOUSEBUTTONDOWN)
	{
		if( event.button.button == SDL_BUTTON_LEFT ) m_Primary = true;
	}
	else if( event.type == SDL_MOUSEBUTTONUP )
	{
		if( event.button.button == SDL_BUTTON_LEFT ) m_Primary = false;
	}

	m_HorizontalAxis = getHorizotnalAxis();
	m_VerticalAxis = getVerticalAxis();
}

glm::vec2 InputManager::GetPrimaryLocation() const
{
	int mouseX = -1;
	int mouseY = -1;

	SDL_GetMouseState(&mouseX, &mouseY);

	return glm::vec2(mouseX, mouseY);
}
