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

TextureContainer FontManager::GetTextureFromFont( SDL_Renderer* renderer, int fontIndex, std::string text, SDL_Color fontColor ) const
{
	SDL_Surface* fontSurface = TTF_RenderText_Solid( GetFont( 0 ), text.c_str(), fontColor );

	if( fontSurface == NULL )
	{
		std::cout << "ERROR Creating FontSurface" << " " << SDL_GetError() << std::endl;
		return { NULL, -1, -1 };
	}

	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface( renderer, fontSurface );

	int fontWidth = fontSurface->w;
	int fontHeight = fontSurface->h;

	SDL_FreeSurface( fontSurface );

	if( fontTexture == NULL )
	{
		std::cout << "ERROR Creating FontTexture" << " " << SDL_GetError() << std::endl;
		return { NULL, -1, -1 };
	}

	return { fontTexture, fontWidth, fontHeight };
}

