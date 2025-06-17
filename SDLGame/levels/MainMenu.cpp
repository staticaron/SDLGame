#include "MainMenu.h"

#include <string>

#include "imgui/imgui.h"

#include "../managers/config.h"

MainMenu::MainMenu()
{
	m_Buttons[0] = { "START", {-1, -1}, NONE, std::bind( &MainMenu::StartGame, this ) };
	m_Buttons[1] = { "ABOUT", {-1, -1}, NONE, std::bind( &MainMenu::StartGame, this ) };
	m_Buttons[2] = { "QUIT", {-1, -1}, NONE, std::bind( &MainMenu::QuitGame, this ) };
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

void MainMenu::Update( double, const InputManager& inputManager )
{
	for( int x = 0; x < m_Buttons.size(); x++ )
	{
		glm::vec2 cursorPosition = inputManager.GetCursorPosition();

		if( pointInRect( m_Buttons.at( x ).dimensions, cursorPosition ) )
		{
			m_Buttons[x].currentState = HOVERED;

			if( inputManager.m_Primary ) m_Buttons.at(x).pressCallback();
		}
		else
		{
			m_Buttons[x].currentState = NONE;
		}
	}
}

void MainMenu::Render( SDL_Renderer* renderer, const TextureManager& textureManager )
{
	SDL_Rect bgRect = { 0, 0, Config::GetWindowSize().x, Config::GetWindowSize().y };
	SDL_RenderCopy( renderer, textureManager.GetBackgroundTexture( 1 ).GetTexture(), NULL, &bgRect );
}

void MainMenu::RenderUI( SDL_Renderer* renderer, const FontManager& fontManager )
{
	for( int x = 0; x < m_Buttons.size(); x++ )
	{
		TextureContainer btnContainer = fontManager.GetTextureFromFont( renderer, 0, m_Buttons.at( x ).text.c_str(), m_Buttons.at( x ).currentState == HOVERED ? m_HoverColor : m_NormalColor );

		float startY = Config::GetWindowSize().y * 0.5f - m_Buttons.size() * btnContainer.GetDimensions().y * 0.5f;
		SDL_Rect fontRect = { Config::GetWindowSize().x * 0.5f - Config::GetWindowPadding() - btnContainer.GetDimensions().x * 0.5f, startY + x * btnContainer.GetDimensions().y, btnContainer.GetDimensions().x, btnContainer.GetDimensions().y };
		m_Buttons[x].dimensions = fontRect;

		SDL_RenderCopy( renderer, btnContainer.GetTexture(), NULL, &fontRect );

		btnContainer.Destroy();
	}
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

void MainMenu::StartGame()
{
	m_StartGame = true;
}

void MainMenu::QuitGame()
{
	m_Quit = true;
}
