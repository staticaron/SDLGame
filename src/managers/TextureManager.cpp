#include "managers/TextureManager.h"

#include "SDL_image.h"

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
	for( auto& pair : m_Textures )
	{
		SDL_DestroyTexture( pair.second.GetTexture() );
	}
}

void TextureManager::LoadAllTextures( SDL_Renderer* renderer )
{
	for( int x = 0; x < m_BgTexturePaths.size(); x++ )
	{
		int width, height = 0;
		SDL_Texture* tex = IMG_LoadTexture( renderer, m_BgTexturePaths.at( x ).c_str() );

		SDL_QueryTexture( tex, NULL, NULL, &width, &height );

		m_BgTextures[x] = { tex, width, height };
	}

	for( int x = 0; x < m_TexturePaths.size(); x++ )
	{
		int width, height;
		SDL_Texture* tex = IMG_LoadTexture( renderer, m_TexturePaths[x].c_str() );

		SDL_QueryTexture( tex, NULL, NULL, &width, &height );

		m_Textures[x] = { tex, width, height };
	}

	for( int x = 0; x < m_LogoPaths.size(); x++ )
	{
		int width, height;
		SDL_Texture* tex = IMG_LoadTexture( renderer, m_LogoPaths[x].c_str() );

		SDL_QueryTexture( tex, NULL, NULL, &width, &height );

		m_Logos[x] = { tex, width, height };
	}
}

const TextureContainer& TextureManager::GetTexture( int index ) const
{
	SDL_assert( index < m_TexturePaths.size() && index >= 0 );

	return m_Textures.at( index );
}

const TextureContainer& TextureManager::GetBackgroundTexture( int index ) const
{
	SDL_assert( index >= 0 && index < m_BgTexturePaths.size() );

	return m_BgTextures.at( index );
}

const TextureContainer& TextureManager::GetLogoTexture( int index ) const
{
	SDL_assert( index >= 0 && index < m_LogoPaths.size() );

	return m_Logos.at( index );
}
