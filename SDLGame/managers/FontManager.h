#pragma once

#include<string>
#include<vector>
#include<map>

#include "SDL/SDL_ttf.h";

#include "TextureManager.h"

class FontManager
{
public:
	FontManager();
	~FontManager();

	TTF_Font* GetFont( int x ) const;

	TextureContainer GetTextureFromFont( SDL_Renderer* renderer, int fontIndex, std::string text, SDL_Color fontColor ) const;

private:
	std::vector<std::string> fontPaths;
	std::map<int, TTF_Font*> fonts;
};