#pragma once

#include "glm.hpp"
#include "mIni/ini.h"

#include <iostream>

class Config
{
public:
	static void Init();

	static void UpdateMaxScore( int highScore );
	static void UpdateMusic( int musicIndex );
	static void UpdateVolume( int musicVolume, int soundVolume );

	static glm::ivec2 GetWindowSize() { return m_WindowSize; }
	static int GetWindowPadding() { return m_WindowPadding; };
	static int GetHighScore() { return m_HighScore; };
	static int GetMusic() { return m_MusicIndex; };
	static int GetSoundVolume() { return m_SoundVolume; };
	static int GetMusicVolume() { return m_MusicVolume; };

private:
	static glm::vec2 m_WindowSize;
	static int m_WindowPadding;

	static int m_HighScore;

	static int m_MusicIndex;
	static int m_SoundVolume;
	static int m_MusicVolume;
};