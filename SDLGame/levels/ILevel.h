#pragma once

#include "SDL/SDL.h"

#include "../managers/FontManager.h"
#include "../managers/TextureManager.h"
#include "../managers/InputManager.h"

class ILevel
{
	public:
	virtual void Update( double, const InputManager& ) = 0;
	virtual void Render( SDL_Renderer*, const TextureManager& ) = 0;
	virtual void RenderImGui() = 0;
	virtual void RenderUI( SDL_Renderer*, const FontManager&, const TextureManager& ) = 0; 
	virtual void RenderTransitions( SDL_Renderer*, const TextureManager& ) = 0;

	virtual void Unload() = 0;
	private:
};