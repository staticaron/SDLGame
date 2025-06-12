#pragma once

#include "glm/glm.hpp"
#include "inih/INIReader.h"

#include <iostream>

class Config
{
public:
	static void Init();
	
	static glm::vec2 GetWindowSize() { return m_WindowSize; }
	static int GetWindowPadding() { return m_WindowPadding; };

private:
	static glm::vec2 m_WindowSize;
	static int m_WindowPadding;
};