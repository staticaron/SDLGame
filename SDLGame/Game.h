#pragma once

#include "SDL/SDL.h"

#include "Level.h"
#include "managers/TextureManager.h"
#include "managers/InputManager.h"

class Game {
	public:
	Game();
	~Game();

	void Run();

	private:
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;

	TextureManager m_TextureManager;
	InputManager m_InputManager;

	Level m_MainLevel = Level();

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double m_DeltaTime = 0;

	void Update(double);
	void HandleCollisions();
	void Render();
};