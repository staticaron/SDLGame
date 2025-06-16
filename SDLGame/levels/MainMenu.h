#pragma once

#include <map>

#include "SDL/SDL.h"

#include "../managers/InputManager.h"
#include "../managers/FontManager.h"
#include "../managers/TextureManager.h"

class MainMenu
{
	public:
	MainMenu();
	~MainMenu();

	void Update( double, const InputManager& );
	void Render( SDL_Renderer*, const TextureManager& );
	void RenderUI( SDL_Renderer*, const FontManager& );
	void RenderImGui( SDL_Renderer* );

	void StartGame();
	void QuitGame();

	bool GetQuitStatus() const { return m_Quit; };

	private:
	std::map<int, SDL_Rect> m_ButtonDimensions;
	bool m_Quit = false;
};