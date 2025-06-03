#include "TextureManager.h"

#include "SDL/SDL_image.h"

TextureManager::TextureManager()
{
	
}

TextureManager::~TextureManager()
{
	for (auto& pair : m_Textures) {
		SDL_DestroyTexture(pair.second.texture);
	}
}

void TextureManager::LoadAllTextures(SDL_Renderer* renderer)
{
	SDL_Texture* bgTex = IMG_LoadTexture(renderer, m_BgTexturePath.c_str());
	int bgWidth, bgHeight;

	SDL_QueryTexture(bgTex, NULL, NULL, &bgWidth, &bgHeight);
	m_Textures[-1] = { bgTex, bgWidth, bgHeight};

	for(int x = 0; x < m_TexturePaths.size(); x++)
	{
		int width, height;
		SDL_Texture* tex = IMG_LoadTexture(renderer, m_TexturePaths[x].c_str());

		SDL_QueryTexture(tex, NULL, NULL, &width, &height);

		m_Textures[x] = {tex, width, height};
	}
}

const TextureContainer& TextureManager::GetTexture(int index) const
{
	SDL_assert(index < m_TexturePaths.size() && index >= 0);

	return m_Textures.at(index);
}
