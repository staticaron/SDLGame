#pragma once

#include "SDL/SDL.h"

class InputManager 
{
	public:
	bool m_Space = false;
	int m_HorizontalAxis = 0;
	int m_VerticalAxis = 0;

	void InitProcessSession();
	void ProcessEvent(SDL_Event);
};