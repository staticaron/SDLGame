#pragma once

#include <map>
#include <vector>
#include <string>

#include <functional>

#include "SDL/SDL.h"

#include "../Level.h"
#include "../managers/InputManager.h"
#include "../managers/FontManager.h"
#include "../managers/TextureManager.h"
#include "../managers/TransitionManager.h"
#include "../levels/ILevel.h"

enum class ButtonState
{
	HOVERED,
	NONE
};

enum class MainMenuAction
{
	STARTGAME, 
	ABOUT,
	QUIT,
	NONE
};

struct MainMenuButton
{
	std::string text;

	SDL_Rect dimensions;
	ButtonState currentState;

	std::function<void()> pressCallback;
};

class MainMenu : public ILevel
{
public:
	MainMenu();
	~MainMenu();

	void Update( double, const InputManager& ) override;
	void Render( SDL_Renderer*, const TextureManager& ) override;
	void RenderUI( SDL_Renderer*, const FontManager&, const TextureManager& ) override;
	void RenderTransitions( SDL_Renderer*, const TextureManager& ) override;
	void RenderImGui() override; 

	void Unload();

	void RenderImGui( SDL_Renderer* );

	void StartButtonCallback();
	void AboutButtonCallback();
	void QuitButtonCallback();

	void StartGame();
	void About();
	void QuitGame();

	MainMenuAction GetMainMenuAction() const { return m_ActionTaken; };

private:
	std::map<int, MainMenuButton> m_Buttons;

	TransitionManager m_TransitionManager;

	MainMenuAction m_ActionTaken = MainMenuAction::NONE;

	SDL_Color m_NormalColor = SDL_Color{ 200, 200, 200, 255 };
	SDL_Color m_HoverColor = SDL_Color{ 100, 100, 100, 255 };
};