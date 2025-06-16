#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "SDL/SDL.h"
#include "glm/glm.hpp"

struct TextureContainer 
{
	public:
	TextureContainer() : m_Texture( NULL ), m_Dimensions( { -1, -1 } ) {}
	TextureContainer(SDL_Texture* texture, int width, int height ) : m_Texture( texture ), m_Dimensions( { width, height } ) {}

	void Destroy()
	{
		if(m_Texture != NULL) SDL_DestroyTexture( m_Texture );
	}

	SDL_Texture* GetTexture() const
	{
		return m_Texture;
	}

	const glm::vec2 GetDimensions() const
	{
		return m_Dimensions;
	}

	private:
	SDL_Texture* m_Texture;
	glm::vec2 m_Dimensions;
};

class TextureManager 
{
	public:
	TextureManager();
	~TextureManager();

	void LoadAllTextures(SDL_Renderer*);

	const TextureContainer& GetTexture(int index) const;
	const TextureContainer& GetBackgroundTexture(int index) const;

	private:
	std::vector<std::string> m_TexturePaths = {
		"assets/textures/ball.png",
		"assets/textures/tex2.png",
		"assets/textures/bat.png",
	};

	std::vector<std::string> m_BgTexturePaths = {
		"assets/textures/bg.png",
		"assets/textures/mainmenu.png"
	};

	std::map<int, TextureContainer> m_Textures;
	std::map<int, TextureContainer> m_BgTextures; 
};