#include "config.h"

#include <string>

glm::vec2 Config::m_WindowSize;
int Config::m_WindowPadding;
int Config::m_HighScore;

void Config::Init()
{
	mINI::INIFile file( "config.ini");

	mINI::INIStructure ini;

	file.read( ini );

	m_WindowPadding = std::stoi( ini["window"]["padding"] );
	m_WindowSize = { std::stoi( ini["window"]["width"]), std::stoi( ini["window"]["height"]) };
	m_HighScore = std::stoi( ini["score"]["highscore"]);
}

void Config::UpdateMaxScore( int highScore )
{
	mINI::INIFile file( "config.ini" );

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
