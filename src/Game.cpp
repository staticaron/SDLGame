#include "Game.h"

#include <chrono>

#include "glm.hpp"

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

	m_Window = SDL_CreateWindow( "Bounze v0.1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Config::GetWindowSize().x, Config::GetWindowSize().y, SDL_WindowFlags::SDL_WINDOW_ALLOW_HIGHDPI );
	m_Renderer = SDL_CreateRenderer( m_Window, -1, 0 );

	IMGUI_CHECKVERSION();

	m_EditorUiContext = ImGui::CreateContext();
	m_InGameUiContext = ImGui::CreateContext();

	ImGui::SetCurrentContext( m_EditorUiContext );

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer( m_Window, m_Renderer );
	ImGui_ImplSDLRenderer2_Init( m_Renderer );
	SDL_SetRenderDrawColor( m_Renderer, 0, 0, 0, 255 );

	ImGui::SetCurrentContext( m_InGameUiContext );

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer( m_Window, m_Renderer );
	ImGui_ImplSDLRenderer2_Init( m_Renderer );
	SDL_SetRenderDrawColor( m_Renderer, 0, 0, 0, 255 );

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	m_TextureManager.LoadAllTextures( m_Renderer );

	AudioManager::Get().Init();
	AudioManager::Get().PlayMusic( Config::GetMusic() );
	AudioManager::Get().LoadVolumes( &m_MasterMusicVolume, &m_MasterSfxVolume );

	// initialize the starting level.
	switch( m_CurrentGameState )
	{
	case MAINMENU:
		m_MainMenuLevel = std::make_unique<MainMenu>(); break;
	case ABOUT:
		m_AboutLevel = std::make_unique<About>(); break;
	case LEVEL:
		m_CurrentLevel = std::make_unique<Level>();
		m_CurrentLevel->InitColliders( m_TextureManager );
		break;
	default:
		m_MainMenuLevel = std::make_unique<MainMenu>(); break;
	}
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

#if DEBUG
		m_DeltaTime = glm::clamp( m_DeltaTime, 0.0, 0.005 );
#endif

		LAST = NOW;

		m_InputManager.InitProcessSession();

		while( SDL_PollEvent( &event ) )
		{
			ImGui::SetCurrentContext( m_EditorUiContext );
			ImGui_ImplSDL2_ProcessEvent( &event );

			ImGui::SetCurrentContext( m_InGameUiContext );
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

		RenderEverything();
	}
}

bool Game::Update( double deltaTime )
{
	switch( m_CurrentGameState )
	{
	case MAINMENU:
		m_MainMenuLevel->Update( deltaTime, m_InputManager );

		if( m_MainMenuLevel->GetMainMenuAction() == MainMenuAction::QUIT ) m_Quit = true;
		else if( m_MainMenuLevel->GetMainMenuAction() == MainMenuAction::STARTGAME )
		{
			ChangeGameState( LEVEL );
			return false;
		}
		else if( m_MainMenuLevel->GetMainMenuAction() == MainMenuAction::ABOUT )
		{
			ChangeGameState( ABOUT );
			return false;
		}
		break;
	case ABOUT:
		m_AboutLevel->Update( deltaTime, m_InputManager );

		if( m_AboutLevel->GetAction() == AboutAction::MAINMENU )
		{
			ChangeGameState( MAINMENU );
			return false;
		}

		break;
	case LEVEL:
		m_CurrentLevel->Update( deltaTime, m_InputManager );
		if( m_CurrentLevel->IsGameOver() )
		{
			m_CurrentLevel->RestartLevel();
			m_CurrentLevel->Update( m_DeltaTime, m_InputManager );
		}
		if( m_CurrentLevel->IsExit() )
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
	case MAINMENU: break;
	case ABOUT: break;
	case LEVEL:
		m_CurrentLevel->HandleCollisions(); break;
	default:
		break;
	}
}

void Game::RenderImGui()
{
	if( !m_ShowImGui ) return;

	ImGui::Begin( "Game Settings" );
	ImGui::SliderFloat( "Music Volume", &m_MasterMusicVolume, 0.0f, 1.0f );
	ImGui::SliderFloat( "SFX Volume ", &m_MasterSfxVolume, 0.0f, 1.0f );
	if( ImGui::Button( "SET" ) )
	{
		AudioManager::Get().SetMusicVolume( m_MasterMusicVolume );
		AudioManager::Get().SetSoundVolume( -1, m_MasterSfxVolume );
	}
	ImGui::InputDouble( "Delta Time", &m_DeltaTime );
	ImGui::End();

	switch( m_CurrentGameState )
	{
	case MAINMENU:
		m_MainMenuLevel->RenderImGui(); break;
	case ABOUT:
		m_AboutLevel->RenderImGui(); break;
	case LEVEL:
		m_CurrentLevel->RenderImGui(); break;
	default:
		break;
	}
}

void Game::RenderEverything()
{
	SDL_RenderClear( m_Renderer );

	RenderGeometry();

	ImGui::SetCurrentContext( m_InGameUiContext );
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	RenderUI();

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData( ImGui::GetDrawData(), m_Renderer );

	RenderTransitions();

	ImGui::SetCurrentContext( m_EditorUiContext );
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	RenderImGui();

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData( ImGui::GetDrawData(), m_Renderer );

	SDL_RenderPresent( m_Renderer );
}

void Game::RenderUI()
{
	switch( m_CurrentGameState )
	{
	case MAINMENU:
		m_MainMenuLevel->RenderUI( m_Renderer, m_FontManager, m_TextureManager ); break;
	case ABOUT:
		m_AboutLevel->RenderUI( m_Renderer, m_FontManager, m_TextureManager ); break;
	case LEVEL:
		m_CurrentLevel->RenderUI( m_Renderer, m_FontManager, m_TextureManager ); break;
	default:
		break;
	}
}

void Game::RenderTransitions()
{
	switch( m_CurrentGameState )
	{
	case MAINMENU:
		m_MainMenuLevel->RenderTransitions( m_Renderer, m_TextureManager ); break;
	case ABOUT:
		m_AboutLevel->RenderTransitions( m_Renderer, m_TextureManager ); break;
	case LEVEL:
		m_CurrentLevel->RenderTransitions( m_Renderer, m_TextureManager ); break;
	default:
		break;
	}
}

void Game::RenderGeometry()
{
	switch( m_CurrentGameState )
	{
	case MAINMENU:
		m_MainMenuLevel->Render( m_Renderer, m_TextureManager ); break;
	case ABOUT:
		m_AboutLevel->Render( m_Renderer, m_TextureManager ); break;
	case LEVEL:
		m_CurrentLevel->Render( m_Renderer, m_TextureManager ); break;
	default:
		break;
	}
}

void Game::ChangeGameState( GameState gameStateToChangeTo )
{
	if( m_CurrentGameState == gameStateToChangeTo ) return;

	if( gameStateToChangeTo == LEVEL )
	{
		if( m_MainMenuLevel != NULL ) m_MainMenuLevel->Unload();
		if( m_AboutLevel != NULL ) m_AboutLevel->Unload();

		m_CurrentLevel = std::make_unique<Level>();
		m_CurrentLevel->InitColliders( m_TextureManager );
		m_CurrentGameState = LEVEL;
	}
	else if( gameStateToChangeTo == MAINMENU )
	{
		if( m_CurrentLevel != NULL ) m_CurrentLevel->Unload();
		if( m_AboutLevel != NULL ) m_AboutLevel->Unload();

		m_MainMenuLevel = std::make_unique<MainMenu>();
		m_CurrentGameState = MAINMENU;
	}
	else if( gameStateToChangeTo == ABOUT )
	{
		if( m_CurrentGameState != NULL ) m_CurrentLevel->Unload();
		if( m_MainMenuLevel != NULL ) m_MainMenuLevel->Unload();

		m_AboutLevel = std::make_unique<About>();
		m_CurrentGameState = ABOUT;
	}
}
