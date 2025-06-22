#include "About.h"

#include <string>
#include <Windows.h>

#include "imgui/imgui.h"

#include "../managers/config.h"
#include "../managers/AudioManager.h"

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
		m_TransitionManager.StartTransition( [this](){ this->m_Action = AboutAction::MAINMENU; } );
	}
}

void About::Render( SDL_Renderer* renderer, const TextureManager& textureManager ) 
{
	auto bgRect = SDL_Rect{ 0, 0, Config::GetWindowSize().x, Config::GetWindowSize().y };
	SDL_RenderCopy( renderer, textureManager.GetBackgroundTexture( 1 ).GetTexture(), NULL, &bgRect);
}

void About::RenderImGui() 
{
	ImGui::Begin( "About Settings" );
	ImGui::InputInt( "Button Width Additive", &m_ButtonWidthAdditive );
	ImGui::End();
}

void About::RenderUI( SDL_Renderer* renderer, const FontManager& fontManager, const TextureManager& textureManager)
{
	std::string aboutHeading = "ABOUT";
	auto headingTex = fontManager.GetTextureFromFont( renderer, 0, aboutHeading.c_str(), m_NormalColor );
	auto headingRect = SDL_Rect( Config::GetWindowSize().x * 0.5f - Config::GetWindowPadding() - headingTex.GetDimensions().x * 0.5f, 50, headingTex.GetDimensions().x, headingTex.GetDimensions().y );

	SDL_RenderCopy( renderer, headingTex.GetTexture(), NULL, &headingRect );

	headingTex.Destroy();

	ImGui::SetNextWindowPos( ImVec2( Config::GetWindowSize().x * 0.5f - Config::GetWindowPadding() - m_MainDimensions.x * 0.5f, Config::GetWindowSize().y * 0.5f - Config::GetWindowPadding() - m_MainDimensions.y * 0.5f ), ImGuiCond_Always );
	ImGui::SetNextWindowSize( ImVec2( m_MainDimensions.x, m_MainDimensions.y ) );
	ImGui::SetNextWindowBgAlpha( 0.0f );

	ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );
	ImGui::Begin( "About Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

	ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 2.0f );

	std::string currentlySelected = "MUSIC " + std::to_string( m_CurrentMusicIndex + 1 );

	ImGui::SetNextItemWidth(-1);

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

	ImGui::PopStyleVar(1);
	
	if( ImGui::Button( "MUSIC SOURCE", {ImGui::GetWindowWidth() + m_ButtonWidthAdditive, 30}) )
	{
		std::string url = "https://swarajthegreat.itch.io/8-bit-arcade-pack";
		ShellExecuteA( NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL );
	}

	ImGui::Separator();

	if( ImGui::Button( "SDL2", { ImGui::GetWindowWidth() + m_ButtonWidthAdditive, 50 } ) )
	{
		std::string url = "https://www.libsdl.org/";
		ShellExecuteA( NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL );
	}

	ImGui::Separator();

	if( ImGui::Button( "Source Code", { ImGui::GetWindowWidth() + m_ButtonWidthAdditive, 50 } ) )
	{
		std::string url = "https://github.com/staticaron/SDLGame";
		ShellExecuteA( NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL );
	}

	ImGui::PopStyleVar(1);

	ImGui::End();
	ImGui::PopStyleVar(1);

	// ImGui::ShowDemoWindow();
}

void About::RenderTransitions( SDL_Renderer* renderer, const TextureManager& textureManager ) 
{
	m_TransitionManager.RenderTransitions( renderer, textureManager );
}

void About::Unload()
{
	std::cout << "About Level Unloaded!" << std::endl;
}
