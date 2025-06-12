#include "FontManager.h"

#include <iostream>

FontManager::FontManager()
{
	fontPaths.push_back("assets/fonts/mightysouly.ttf");

	if( TTF_Init() < 0 )
	{
		std::cout << "ERROR Initializing TTF library : " << TTF_GetError() << std::endl;
		return;
	}

	for( int x = 0; x < fontPaths.size(); x++ )
	{
		fonts[x] = TTF_OpenFont(fontPaths[x].c_str(), 72);
		if( fonts[x] == NULL ) std::cout << "ERROR loading the font at : " << fontPaths[x] << " " << TTF_GetError() << std::endl;
	}
}

FontManager::~FontManager(){}

TTF_Font* FontManager::GetFont( int x ) const
{
	SDL_assert( x > 0 && x < fonts.size() );
	return fonts.at(x);
}

