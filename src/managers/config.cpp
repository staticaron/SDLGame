#include "managers/config.h"

#include <filesystem>
#include <string>

glm::vec2 Config::m_WindowSize;
int Config::m_WindowPadding;
int Config::m_HighScore;
int Config::m_MusicIndex;
int Config::m_SoundVolume;
int Config::m_MusicVolume;

void Config::Init()
{
	mINI::INIFile file( RESOURCE_PATH "config.ini" );

	mINI::INIStructure ini;

	file.read( ini );

	m_WindowPadding = std::stoi( ini["window"]["padding"] );
	m_WindowSize = { std::stoi( ini["window"]["width"] ), std::stoi( ini["window"]["height"] ) };
	m_HighScore = std::stoi( ini["score"]["highscore"] );
	m_MusicIndex = std::stoi( ini["music"]["index"] );
	m_MusicVolume = std::stoi( ini["music"]["volume"] );
	m_SoundVolume = std::stoi( ini["sound"]["volume"] );
}

void Config::UpdateMaxScore( int highScore )
{
	mINI::INIFile file( RESOURCE_PATH "config.ini" );

	mINI::INIStructure ini;

	file.read( ini );

	if( highScore > std::stoi( ini["score"]["highscore"] ) )
	{
		ini["score"]["highscore"] = std::to_string( highScore );
		m_HighScore = highScore;
		file.write( ini );
	}

	std::cout << "config.h written successfully!" << std::endl;
}

void Config::UpdateMusic( int musicIndex )
{
	if( musicIndex == m_MusicIndex ) return;

	mINI::INIFile file( "config.ini" );

	mINI::INIStructure ini;

	file.read( ini );

	ini["music"]["index"] = std::to_string( musicIndex );
	file.write( ini );

	std::cout << "config.h written successfully!" << std::endl;
}

void Config::UpdateVolume( int musicVolume = -1, int soundVolume = -1 )
{
	if( musicVolume == m_MusicVolume && soundVolume == m_SoundVolume ) return;

	mINI::INIFile file( "config.ini" );

	mINI::INIStructure ini;

	file.read( ini );

	if( musicVolume != -1 ) ini["music"]["volume"] = std::to_string( musicVolume );
	if( soundVolume != -1 ) ini["sound"]["volume"] = std::to_string( soundVolume );
	file.write( ini );

	std::cout << "config.h written successfully!" << std::endl;
}
