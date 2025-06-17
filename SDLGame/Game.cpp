#include "Game.h"

#include <chrono>

#include "managers/config.h"
#include "managers/AudioManager.h"

Game::Game()
{
	Config::Init();

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER ) != 0 )
	{
		printf( "Error: %s\n", SDL_GetError() );
		return;
	}

	m_Window = SDL_CreateWindow( "SDL!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Config::GetWindowSize().x, Config::GetWindowSize().y, SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI );
	m_Renderer = SDL_CreateRenderer( m_Window, -1, 0 );

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForSDLRenderer( m_Window, m_Renderer );
	ImGui_ImplSDLRenderer2_Init( m_Renderer );

	SDL_SetRenderDrawColor( m_Renderer, 0, 0, 0, 255 );

	m_TextureManager.LoadAllTextures( m_Renderer );

	AudioManager::Get().Init();
	AudioManager::Get().PlayMusic( 0 );

	// m_CurrentLevel = std::make_unique<Level>();
	m_MainMenuLevel = std::make_unique<MainMenu>();

	// m_CurrentLevel->InitColliders( m_TextureManager );
}

Game::~Game()
{
	SDL_DestroyRenderer( m_Renderer );
}

void Game::Run()
{
	SDL_Event event;

	while( !m_Quit )
	{
		NOW = SDL_GetPerformanceCounter();
		m_DeltaTime = (double)(NOW - LAST) / (double)SDL_GetPerformanceFrequency();
		LAST = NOW;

		m_InputManager.InitProcessSession();

		while( SDL_PollEvent( &event ) )
		{
			ImGui_ImplSDL2_ProcessEvent( &event );

			if( event.type == SDL_QUIT )
			{
				m_Quit = true;
				break;
			}

			m_InputManager.ProcessEvent( event );

			if( m_InputManager.m_Tab ) m_ShowImGui = !m_ShowImGui;
		}

		if( m_Quit ) break;

		if( !Update( m_DeltaTime ) ) continue;

		if( m_Quit ) break;

		HandleCollisions();

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		RenderImGui();
		RenderEverything();
	}
}

bool Game::Update( double deltaTime )
{
	switch( m_CurrentGameState )
	{
	case MAINMENU:
		m_MainMenuLevel->Update( deltaTime, m_InputManager );

		if( m_MainMenuLevel->GetQuitStatus() ) m_Quit = true;
		else if( m_MainMenuLevel->GetStartGameStatus() )
		{
			ChangeGameState( LEVEL );
		}
		break;
	case LEVEL:
		m_CurrentLevel->Update( deltaTime, m_InputManager );
		if( m_CurrentLevel->IsGameOver() )
		{
			m_CurrentLevel->RestartLevel();
			m_CurrentLevel->Update( m_DeltaTime, m_InputManager );
		}
		if ( m_CurrentLevel->IsExit() ) 
		{
			ChangeGameState( MAINMENU );
			return false;
		}
		break;
	default:
		break;
	}

	return true;
}

void Game::HandleCollisions()
{
	switch( m_CurrentGameState )
	{
	case MAINMENU:
		break;
	case LEVEL:
		m_CurrentLevel->HandleCollisions();
		break;
	default:
		break;
	}
}

void Game::RenderImGui()
{
	if( !m_ShowImGui ) return;

	ImGui::Begin( "Game Settings" );
	ImGui::SliderFloat( "Music Volume", &m_MasterVolume, 0.0f, 1.0f );
	ImGui::SliderFloat( "SFX Volume ", &m_MasterSfxVolume, 0.0f, 1.0f );
	if( ImGui::Button( "SET" ) )
	{
		AudioManager::Get().SetMusicVolume( m_MasterVolume );
		AudioManager::Get().SetSoundVolume( -1, m_MasterSfxVolume );
	}
	ImGui::InputDouble( "Delta Time", &m_DeltaTime );
	ImGui::End();

	switch( m_CurrentGameState )
	{
	case MAINMENU:
		m_MainMenuLevel->RenderImGui( m_Renderer );
		break;
	case LEVEL:
		m_CurrentLevel->RenderImGui();
		break;
	default:
		break;
	}

}

void Game::RenderEverything()
{
	ImGui::Render();

	SDL_RenderClear( m_Renderer );

	RenderGeometry();

	RenderUI();

	RenderTransitions();

	ImGui_ImplSDLRenderer2_RenderDrawData( ImGui::GetDrawData(), m_Renderer );

	SDL_RenderPresent( m_Renderer );
}

void Game::RenderUI()
{
	switch( m_CurrentGameState )
	{
	case MAINMENU:
		m_MainMenuLevel->RenderUI( m_Renderer, m_FontManager );
		break;
	case LEVEL:
		m_CurrentLevel->RenderUI( m_Renderer, m_FontManager );
		break;
	default:
		break;
	}
}

void Game::RenderTransitions()
{
	if( m_CurrentGameState == LEVEL )
	{
		m_CurrentLevel->RenderTransitions( m_Renderer, m_TextureManager );
	}
	else if( m_CurrentGameState == MAINMENU )
	{
		m_MainMenuLevel->RenderTransitions( m_Renderer, m_TextureManager );
	}
}

void Game::RenderGeometry()
{
	switch( m_CurrentGameState )
	{
	case MAINMENU:
		m_MainMenuLevel->Render( m_Renderer, m_TextureManager );
		break;
	case LEVEL:
		m_CurrentLevel->Render( m_Renderer, m_TextureManager );
		break;
	default:
		break;
	}
}

void Game::ChangeGameState( GameState gameStateToChangeTo )
{
	if( m_CurrentGameState == gameStateToChangeTo ) return;

	if( gameStateToChangeTo == LEVEL )
	{
		m_CurrentLevel = std::make_unique<Level>();
		m_CurrentLevel->InitColliders( m_TextureManager );
		m_CurrentGameState = LEVEL;
	}
	else if ( gameStateToChangeTo == MAINMENU )
	{
		if( m_CurrentLevel != NULL ) m_CurrentLevel->Unload();
		m_MainMenuLevel = std::make_unique<MainMenu>();
		m_CurrentGameState = MAINMENU;
	}
}
