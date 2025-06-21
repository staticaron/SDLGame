#include "About.h"

#include <string>

#include "imgui/imgui.h"

#include "../managers/config.h"

About::About()
{
	m_TransitionManager.Init( TransitionState::START );
	m_TransitionManager.StartTransition( []() {} );
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
	ImGui::SetNextWindowPos( ImVec2( Config::GetWindowSize().x * 0.5f - Config::GetWindowPadding() - m_MainDimensions.x * 0.5f, Config::GetWindowSize().y * 0.5f - Config::GetWindowPadding() - m_MainDimensions.y * 0.5f ), ImGuiCond_Always);
	ImGui::SetNextWindowSize( ImVec2( m_MainDimensions.x, m_MainDimensions.y ) );
	ImGui::SetNextWindowBgAlpha( 0.3f );
	ImGui::Begin( "About Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove );

	std::string currentlySelected = "MUSIC " + std::to_string( m_CurrentMusicIndex+1 ) ;

	ImGui::Text("Background MUSIC");
	ImGui::SameLine();

	if( ImGui::BeginCombo( "##Background MUSIC", currentlySelected.c_str() ) )
	{
		for( int x = 0; x < m_MusicCount; x++ )
		{
			std::string itemName = "MUSIC" + std::to_string(x+1);
			bool isSelected = m_CurrentMusicIndex == x;
			if( ImGui::Selectable( itemName.c_str(), isSelected) ) 
			{
				m_CurrentMusicIndex = x;
				currentlySelected = itemName;
			}

			if( isSelected ) ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::End();
}

void About::RenderUI( SDL_Renderer* renderer, const FontManager& textureManager )
{
	std::string aboutHeading = "ABOUT";
	auto headingTex = textureManager.GetTextureFromFont( renderer, 0, aboutHeading.c_str(), m_NormalColor );
	auto headingRect = SDL_Rect( Config::GetWindowSize().x * 0.5f - Config::GetWindowPadding() - headingTex.GetDimensions().x * 0.5f, 0, headingTex.GetDimensions().x, headingTex.GetDimensions().y );

	SDL_RenderCopy( renderer, headingTex.GetTexture(), NULL, &headingRect );

	headingTex.Destroy();
}

void About::RenderTransitions( SDL_Renderer* renderer, const TextureManager& textureManager ) 
{
	m_TransitionManager.RenderTransitions( renderer, textureManager );
}

void About::Unload()
{
	std::cout << "About Level Unloaded!" << std::endl;
}
