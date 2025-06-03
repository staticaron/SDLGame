#include "Game.h"

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	m_Window = SDL_CreateWindow("SDL!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI);
	m_Renderer = SDL_CreateRenderer(m_Window, -1, 0);

	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);

	m_TextureManager.LoadAllTextures(m_Renderer);

	m_MainLevel.InitColliders(m_TextureManager);
}

Game::~Game()
{
	SDL_DestroyRenderer(m_Renderer);
}

void Game::Run()
{
	bool isRunning = true;

	SDL_Event event;

	while (isRunning)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		m_DeltaTime = (double)(NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency();

		m_InputManager.InitProcessSession();

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) {
				isRunning = false;
				break;
			}

			m_InputManager.ProcessEvent(event);	
		}

		if(!isRunning) break;

		Update(m_DeltaTime);
		HandleCollisions();
		Render();
	}
}

void Game::Update(double deltaTime)
{
	m_MainLevel.Update(deltaTime, m_InputManager);
}

void Game::HandleCollisions() 
{
	m_MainLevel.HandleCollisions();
}

void Game::Render()
{
	SDL_RenderClear(m_Renderer);

	SDL_Rect bgRect = { 0, 0, 800, 600 };
	SDL_RenderCopy(m_Renderer, m_TextureManager.GetTexture(-1).texture, NULL, &bgRect);

	m_MainLevel.Render(m_Renderer, m_TextureManager);

	SDL_RenderPresent(m_Renderer);
}
