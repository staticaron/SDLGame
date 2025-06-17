#pragma once

#include "SDL/SDL.h"
#include "glm/glm.hpp"

class InputManager 
{
	public:
	bool m_Space = false;
	bool m_Tab = false;
	bool m_Escape = false;

	int m_HorizontalAxis = 0;
	int m_VerticalAxis = 0;

	bool m_Primary = false;

	void InitProcessSession();
	void ProcessEvent(SDL_Event);

	glm::vec2 GetPrimaryLocation() const;
};