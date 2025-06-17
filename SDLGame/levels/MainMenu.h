#pragma once

#include <map>
#include <vector>
#include <string>

#include <functional>

#include "SDL/SDL.h"

#include "../managers/InputManager.h"
#include "../managers/FontManager.h"
#include "../managers/TextureManager.h"

enum ButtonState
{
	HOVERED,
	NONE
};

struct MainMenuButton
{
	std::string text;

	SDL_Rect dimensions;
	ButtonState currentState;

	std::function<void()> pressCallback;
};

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
	bool GetStartGameStatus() const { return m_StartGame; };

private:
	std::map<int, MainMenuButton> m_Buttons;

	bool m_Quit = false;
	bool m_StartGame = false;

	SDL_Color m_NormalColor = SDL_Color{ 200, 200, 200, 255 };
	SDL_Color m_HoverColor = SDL_Color{ 100, 100, 100, 255 };
};