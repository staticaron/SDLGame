#pragma once

#include "glm/glm.hpp"
#include "inih/INIReader.h"
#include "mIni/ini.h"

#include <iostream>

class Config
{
public:
	static void Init();
	
	static void UpdateMaxScore( int highScore );

	static glm::vec2 GetWindowSize() { return m_WindowSize; }
	static int GetWindowPadding() { return m_WindowPadding; };
	static int GetHighScore() { return m_HighScore; };

private:
	static glm::vec2 m_WindowSize;
	static int m_WindowPadding;
	static int m_HighScore;
};