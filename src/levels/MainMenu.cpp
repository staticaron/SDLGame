#include "levels/MainMenu.h"

#include <string>

#include "imgui.h"

#include "managers/config.h"

MainMenu::MainMenu()
{
	m_Buttons[0] = { "START", {-1, -1}, ButtonState::NONE, [this]() { this->StartButtonCallback(); } };
	m_Buttons[1] = { "ABOUT", {-1, -1}, ButtonState::NONE, [this]() { this->AboutButtonCallback(); } };
	m_Buttons[2] = { "QUIT", {-1, -1}, ButtonState::NONE, [this]() { this->QuitButtonCallback(); } };

	m_TransitionManager.Init( TransitionState::START );
	m_TransitionManager.StartTransition( []() {} );
}

MainMenu::~MainMenu()
{

}

bool pointInRect( const SDL_Rect& rect, const glm::vec2& point )
{
	bool xOverlap = point.x >= rect.x && point.x < rect.x + rect.w;
	bool yOverlap = point.y >= rect.y && point.y < rect.y + rect.h;

	return xOverlap && yOverlap;
}

void MainMenu::Update( double deltaTime, const InputManager& inputManager )
{
	if( m_TransitionManager.IsTransitioning() )
	{
		m_TransitionManager.Update( deltaTime );
		return;
	}

	for( int x = 0; x < m_Buttons.size(); x++ )
	{
		glm::vec2 cursorPosition = inputManager.GetCursorPosition();

		if( pointInRect( m_Buttons.at( x ).dimensions, cursorPosition ) )
		{
			m_Buttons[x].currentState = ButtonState::HOVERED;

			if( inputManager.m_Primary ) m_Buttons.at( x ).pressCallback();
		}
		else m_Buttons[x].currentState = ButtonState::NONE;
	}

}

void MainMenu::Render( SDL_Renderer* renderer, const TextureManager& textureManager )
{
	SDL_Rect bgRect = { 0, 0, Config::GetWindowSize().x, Config::GetWindowSize().y };
	SDL_RenderCopy( renderer, textureManager.GetBackgroundTexture( 1 ).GetTexture(), NULL, &bgRect );
}

void MainMenu::RenderUI( SDL_Renderer* renderer, const FontManager& fontManager, const TextureManager& textureManager )
{
	for( int x = 0; x < m_Buttons.size(); x++ )
	{
		TextureContainer btnContainer = fontManager.GetTextureFromFont( renderer, 0, m_Buttons.at( x ).text.c_str(), m_Buttons.at( x ).currentState == ButtonState::HOVERED ? m_HoverColor : m_NormalColor );

		float startY = Config::GetWindowSize().y * 0.5f - m_Buttons.size() * btnContainer.GetDimensions().y * 0.5f + m_MenuYAdditive;
		SDL_Rect fontRect = { Config::GetWindowSize().x * 0.5f - Config::GetWindowPadding() - btnContainer.GetDimensions().x * 0.5f, startY + x * btnContainer.GetDimensions().y, btnContainer.GetDimensions().x, btnContainer.GetDimensions().y };
		m_Buttons[x].dimensions = fontRect;

		SDL_RenderCopy( renderer, btnContainer.GetTexture(), NULL, &fontRect );

		btnContainer.Destroy();
	}
}

void MainMenu::RenderTransitions( SDL_Renderer* renderer, const TextureManager& textureManager )
{
	m_TransitionManager.RenderTransitions( renderer, textureManager );
}

void MainMenu::RenderImGui()
{
	ImGui::Begin( "Main Menu Settings" );
	ImGui::InputInt( "Menu Y Additive", &m_MenuYAdditive );
	ImGui::End();
}

void MainMenu::Unload()
{
	std::cout << "MainMenu Unloaded!" << std::endl;
}

void MainMenu::RenderImGui( SDL_Renderer* )
{
	for( auto& [index, buttonDetails] : m_Buttons )
	{
		ImVec2 min = { (float)buttonDetails.dimensions.x, (float)buttonDetails.dimensions.y };
		ImVec2 max = { (float)(buttonDetails.dimensions.x + buttonDetails.dimensions.w), (float)(buttonDetails.dimensions.y + buttonDetails.dimensions.h) };
		ImGui::GetBackgroundDrawList()->AddRect( min, max, IM_COL32( 255, 0, 0, 255 ), 0, 0, 2.0f );
	}
}

void MainMenu::StartButtonCallback()
{
	m_TransitionManager.Init( TransitionState::END );
	m_TransitionManager.StartTransition( [this]() { this->StartGame(); } );
}

void MainMenu::AboutButtonCallback()
{
	m_TransitionManager.Init( TransitionState::END );
	m_TransitionManager.StartTransition( [this]() { this->About(); } );
}

void MainMenu::QuitButtonCallback()
{
	m_TransitionManager.Init( TransitionState::END );
	m_TransitionManager.StartTransition( [this]() { this->QuitGame(); } );
}

void MainMenu::StartGame()
{
	m_ActionTaken = MainMenuAction::STARTGAME;
}

void MainMenu::About()
{
	m_ActionTaken = MainMenuAction::ABOUT;
}

void MainMenu::QuitGame()
{
	m_ActionTaken = MainMenuAction::QUIT;
}
