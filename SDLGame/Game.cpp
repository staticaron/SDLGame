#include "Game.h"

#include "managers/config.h"

Game::Game()
{
	Config::Init();

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER ) != 0 )
	{
		printf( "Error: %s\n", SDL_GetError() );
		return;
	}

	m_Window = SDL_CreateWindow( "SDL!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Config::GetWindowSize().x, Config::GetWindowSize().y, SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI);
	m_Renderer = SDL_CreateRenderer( m_Window, -1, 0 );

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForSDLRenderer( m_Window, m_Renderer );
	ImGui_ImplSDLRenderer2_Init( m_Renderer );

	SDL_SetRenderDrawColor( m_Renderer, 0, 0, 0, 255 );

	m_TextureManager.LoadAllTextures( m_Renderer );

	m_MainLevel.InitColliders( m_TextureManager );
}

Game::~Game()
{
	SDL_DestroyRenderer( m_Renderer );
}

void Game::Run()
{
	bool isRunning = true;

	SDL_Event event;

	while( isRunning )
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		m_DeltaTime = (double)(NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency();

		m_InputManager.InitProcessSession();

		while( SDL_PollEvent( &event ) )
		{
			ImGui_ImplSDL2_ProcessEvent( &event );

			if( event.type == SDL_QUIT )
			{
				isRunning = false;
				break;
			}

			if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE )
			{
				m_ShowImGui = !m_ShowImGui;
			}

			m_InputManager.ProcessEvent( event );
		}

		if( !isRunning ) break;

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		if(m_MainLevel.IsGameOver()) 
		{
			// Game Over. Load the Game Over Level
			// m_MainLevel = gameOverLevel;
		}

		Update( m_DeltaTime );
		HandleCollisions();
		RenderImGui();
		RenderEverything();
	}
}

void Game::Update( double deltaTime )
{
	m_MainLevel.Update( deltaTime, m_InputManager );
}

void Game::HandleCollisions()
{
	m_MainLevel.HandleCollisions();
}

void Game::RenderImGui()
{
	if( !m_ShowImGui ) return;
	m_MainLevel.RenderImGui();
}

void Game::RenderEverything()
{
	ImGui::Render();

	SDL_RenderClear( m_Renderer );

	// Rendering Background
	SDL_Rect bgRect = { 0, 0, Config::GetWindowSize().x, Config::GetWindowSize().y };
	SDL_RenderCopy( m_Renderer, m_TextureManager.GetTexture( -1 ).texture, NULL, &bgRect );

	// Rendering the level contents
	m_MainLevel.Render( m_Renderer, m_TextureManager );

	// Render the Level UI
	RenderUI();

	ImGui_ImplSDLRenderer2_RenderDrawData( ImGui::GetDrawData(), m_Renderer );

	SDL_RenderPresent( m_Renderer );
}

void Game::RenderUI(){ m_MainLevel.RenderUI(m_Renderer, m_FontManager); }
