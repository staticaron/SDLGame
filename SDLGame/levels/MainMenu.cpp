#include "MainMenu.h"

#include <string>

#include "imgui/imgui.h"

#include "../managers/config.h"

MainMenu::MainMenu()
{
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
	if( inputManager.m_Primary )
	{
		glm::vec2 cursorPosition = inputManager.GetPrimaryLocation();

		std::cout << "Clicked at : " << cursorPosition.x << ", " << cursorPosition.y << std::endl;

		for( int x = 0; x < m_ButtonDimensions.size(); x++ )
		{
			if( !pointInRect( m_ButtonDimensions.at( x ), cursorPosition ) ) continue;

			switch( x )
			{
			case 0:
				StartGame();
				break;
			case 1:
				QuitGame();
				break;
			default:
				break;
			}
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
	std::string startGameText = "START";
	TextureContainer startContainer = fontManager.GetTextureFromFont( renderer, 0, startGameText.c_str(), SDL_Color{ 200, 200, 200 } );

	SDL_Rect startFontRect = { Config::GetWindowSize().x * 0.5f - Config::GetWindowPadding() - startContainer.GetDimensions().x * 0.5f, Config::GetWindowSize().y * 0.5 - startContainer.GetDimensions().y, startContainer.GetDimensions().x, startContainer.GetDimensions().y };
	m_ButtonDimensions[0] = startFontRect;

	SDL_RenderCopy( renderer, startContainer.GetTexture(), NULL, &startFontRect );

	startContainer.Destroy();

	std::string quitText = "EXIT";
	TextureContainer exitContainer = fontManager.GetTextureFromFont( renderer, 0, quitText.c_str(), SDL_Color{ 200, 200, 200 } );

	SDL_Rect quitFontRect = { Config::GetWindowSize().x * 0.5f - Config::GetWindowPadding() - exitContainer.GetDimensions().x * 0.5f, Config::GetWindowSize().y * 0.5, exitContainer.GetDimensions().x, exitContainer.GetDimensions().y };
	m_ButtonDimensions[1] = quitFontRect;

	SDL_RenderCopy( renderer, exitContainer.GetTexture(), NULL, &quitFontRect );

	exitContainer.Destroy();
}

void MainMenu::RenderImGui( SDL_Renderer* )
{
	for( auto& [index, rect] : m_ButtonDimensions )
	{
		ImVec2 min = { (float)rect.x, (float)rect.y };
		ImVec2 max = { (float)(rect.x + rect.w), (float)(rect.y + rect.h) };
		ImGui::GetBackgroundDrawList()->AddRect( min, max, IM_COL32( 255, 0, 0, 255 ), 0, 0, 2.0f );
	}
}

void MainMenu::StartGame()
{
	std::cout << "Load the game" << std::endl;
}

void MainMenu::QuitGame()
{
	m_Quit = true;
}
