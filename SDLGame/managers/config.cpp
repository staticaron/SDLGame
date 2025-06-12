#include "config.h"

#include <filesystem>

glm::vec2 Config::m_WindowSize;
int Config::m_WindowPadding;

void Config::Init()
{
	INIReader reader = INIReader( "config.ini" );

	if( reader.ParseError() < 0 )
	{
		std::cout << "Error while parsing config.ini file " << std::endl;
		return;
	}

	m_WindowSize = { reader.GetInteger( "window", "width", -1 ), reader.GetInteger( "window", "height", -1 ) };
	m_WindowPadding = reader.GetInteger( "window", "padding", -1 );
}
