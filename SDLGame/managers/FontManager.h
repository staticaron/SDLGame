#pragma once

#include<string>
#include<vector>
#include<map>

#include "SDL/SDL_ttf.h";

class FontManager
{
public:
	FontManager();
	~FontManager();

	TTF_Font* GetFont( int x ) const;

private:
	std::vector<std::string> fontPaths;
	std::map<int, TTF_Font*> fonts;
};