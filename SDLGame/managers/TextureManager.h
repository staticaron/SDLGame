#pragma once

#include <vector>
#include <map>
#include <string>

#include "SDL/SDL.h"

struct TextureContainer {
	SDL_Texture* texture;
	int width, height;
};

class TextureManager 
{
	public:
	TextureManager();
	~TextureManager();

	void LoadAllTextures(SDL_Renderer*);
	const TextureContainer& GetTexture(int index) const;

	private:
	std::string m_BgTexturePath = "assets/textures/bg.png";

	std::vector<std::string> m_TexturePaths = {
		"assets/textures/ball.png",
		"assets/textures/tex2.png",
		"assets/textures/bat.png",
	};

	std::map<int, TextureContainer> m_Textures;
};