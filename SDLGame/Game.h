#pragma once

#include "SDL/SDL.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"

#include "Level.h"
#include "levels/MainMenu.h"
#include "managers/TextureManager.h"
#include "managers/InputManager.h"
#include "managers/ScoreManager.h"
#include "managers/FontManager.h"

enum GameState
{
	MAINMENU,
	LEVEL
};

class Game
{
public:
	Game();
	~Game();

	void Run();

private:
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;

	TextureManager m_TextureManager;
	FontManager m_FontManager;
	InputManager m_InputManager;
	ScoreManager m_ScoreManager;

	std::unique_ptr<MainMenu> m_MainMenuLevel = NULL;
	std::unique_ptr<Level> m_CurrentLevel = NULL;

	Uint64 NOW = 0;
	Uint64 LAST = SDL_GetPerformanceCounter();;
	double m_DeltaTime = 0;

	bool m_ShowImGui = false;

	float m_MasterVolume = 0.5f;
	float m_MasterSfxVolume = 0.5f;

	GameState m_CurrentGameState = MAINMENU;

	bool Update( double );
	void HandleCollisions();
	void RenderImGui();
	void RenderEverything();
	void RenderUI();
	void RenderGeometry();

	void ChangeGameState( GameState );
};