#include "levels/About.h"

#include <cstdlib>
#include <string>

#include "imgui.h"

#include "managers/AudioManager.h"
#include "managers/config.h"

About::About()
{
	m_TransitionManager.Init( TransitionState::START );
	m_TransitionManager.StartTransition( []() {} );

	m_MainDimensions = { 300, 230 };
}

About::~About() {}

void About::Update( double deltaTime, const InputManager& inputManager )
{
	m_TransitionManager.Update( deltaTime );

	if( inputManager.m_Escape )
	{
		m_TransitionManager.Init( TransitionState::END );
		m_TransitionManager.StartTransition( [this]() { this->m_Action = AboutAction::MAINMENU; } );
	}
}

void About::Render( SDL_Renderer* renderer, const TextureManager& textureManager )
{
	auto bgRect = SDL_Rect{ 0, 0, Config::GetWindowSize().x, Config::GetWindowSize().y };
	SDL_RenderCopy( renderer, textureManager.GetBackgroundTexture( 4 ).GetTexture(), NULL, &bgRect );
}

void About::RenderImGui()
{
	ImGui::Begin( "About Settings" );
	ImGui::InputInt( "Buttons Width Additive", &m_ButtonWidthAdditive );
	ImGui::InputInt( "Buttons X Additive", &m_ButtonXAdditive );
	ImGui::InputInt( "Buttons Y Additive", &m_ButtonYAdditive );
	ImGui::InputInt( "About Header Y Additive", &m_AboutHeaderYAdditive );
	ImGui::End();
}

void openUrlAllPlatforms( std::string url )
{
#ifdef _WIN32
	std::string command = "start ";
	command += url;
	system( command.c_str() );
#elif __linux__
	std::string command = "xdg-open ";
	command += url;
	system( command.c_str() );
#else
#error "Unsupported OS"
#endif
}

void About::RenderUI( SDL_Renderer* renderer, const FontManager& fontManager, const TextureManager& textureManager )
{
	ImGui::SetNextWindowPos( ImVec2( Config::GetWindowSize().x * 0.5f - Config::GetWindowPadding() - m_MainDimensions.x * 0.5f + m_ButtonXAdditive, Config::GetWindowSize().y * 0.5f - Config::GetWindowPadding() - m_MainDimensions.y * 0.5f + m_ButtonYAdditive ), ImGuiCond_Always );
	ImGui::SetNextWindowSize( ImVec2( m_MainDimensions.x, m_MainDimensions.y ) );
	ImGui::SetNextWindowBgAlpha( 0.0f );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
	ImGui::Begin( "About Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

	ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 2.0f );

	std::string currentlySelected = "MUSIC " + std::to_string( m_CurrentMusicIndex + 1 );

	ImGui::SetNextItemWidth( -1 );

	ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2(
		ImGui::GetStyle().FramePadding.x,
		12.0f
	) );

	if( ImGui::BeginCombo( "##Background MUSIC", currentlySelected.c_str() ) )
	{
		for( int x = 0; x < AudioManager::Get().GetMusicCount(); x++ )
		{
			std::string itemName = "MUSIC " + std::to_string( x + 1 );
			bool isSelected = m_CurrentMusicIndex == x;
			if( ImGui::Selectable( itemName.c_str(), isSelected ) )
			{
				m_CurrentMusicIndex = x;
				currentlySelected = itemName;
				AudioManager::Get().PlayMusic( x );
			}

			if( isSelected ) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::PopStyleVar( 1 );

	if( ImGui::Button( "MUSIC SOURCE", { ImGui::GetWindowWidth() + m_ButtonWidthAdditive, 30 } ) )
	{
		openUrlAllPlatforms( "https://swarajthegreat.itch.io/8-bit-arcade-pack" );
	}

	ImGui::Separator();

	if( ImGui::Button( "SDL2", { ImGui::GetWindowWidth() + m_ButtonWidthAdditive, 50 } ) )
	{
		openUrlAllPlatforms( "https://www.libsdl.org/" );
	}

	ImGui::Separator();

	if( ImGui::Button( "Source Code", { ImGui::GetWindowWidth() + m_ButtonWidthAdditive, 50 } ) )
	{
		openUrlAllPlatforms( "https://github.com/staticaron/SDLGame" );
	}

	ImGui::PopStyleVar( 1 );

	ImGui::End();
	ImGui::PopStyleVar( 1 );
}

void About::RenderTransitions( SDL_Renderer* renderer, const TextureManager& textureManager )
{
	m_TransitionManager.RenderTransitions( renderer, textureManager );
}

void About::Unload()
{
	std::cout << "About Level Unloaded!" << std::endl;
}
