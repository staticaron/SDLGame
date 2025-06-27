#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "glm.hpp"
#include "SDL.h"

struct TextureContainer
{
public:
	TextureContainer() : m_Texture( NULL ), m_Dimensions( { -1, -1 } ) {}
	TextureContainer( SDL_Texture* texture, int width, int height ) : m_Texture( texture ), m_Dimensions( { width, height } ) {}

	void Destroy() const
	{
		if( m_Texture != NULL ) SDL_DestroyTexture( m_Texture );
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

	void LoadAllTextures( SDL_Renderer* );

	const TextureContainer& GetTexture( int index ) const;
	const TextureContainer& GetBackgroundTexture( int index ) const;
	const TextureContainer& GetLogoTexture( int index ) const;

private:
	std::vector<std::string> m_TexturePaths = {
		RESOURCE_PATH "textures/ball.png",
		RESOURCE_PATH "textures/tex2.png",
		RESOURCE_PATH "textures/bat.png",
		RESOURCE_PATH "textures/bat_large.png",
		RESOURCE_PATH "textures/diamond.png"
	};

	std::vector<std::string> m_BgTexturePaths = {
		RESOURCE_PATH "textures/bg.png",
		RESOURCE_PATH "textures/mainmenu.png",
		RESOURCE_PATH "textures/transition.png",
		RESOURCE_PATH "textures/bg2.png",
		RESOURCE_PATH "textures/about.png"
	};

	std::vector<std::string> m_LogoPaths = {
		RESOURCE_PATH "textures/logos/sdl.png"
	};

	std::map<int, TextureContainer> m_Textures;
	std::map<int, TextureContainer> m_BgTextures;
	std::map<int, TextureContainer> m_Logos;
};