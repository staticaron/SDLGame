#pragma once

#include <string>

#include "../managers/TransitionManager.h"
#include "../levels/ILevel.h"

enum class AboutAction
{
	MAINMENU, NONE
};

class About : public ILevel
{
public:
	About();
	~About();

	void Update( double, const InputManager& ) override;
	void Render( SDL_Renderer*, const TextureManager& ) override;
	void RenderImGui() override;
	void RenderUI( SDL_Renderer*, const FontManager&, const TextureManager&  ) override;
	void RenderTransitions( SDL_Renderer*, const TextureManager& ) override;

	void Unload() override;

	AboutAction GetAction() const { return m_Action; };

private:
	glm::ivec2 m_MainDimensions = {0, 0};

	AboutAction m_Action = AboutAction::NONE;

	TransitionManager m_TransitionManager;

	int m_CurrentMusicIndex = 0;

	int m_ButtonWidthAdditive = -17;
	int m_ButtonYAdditive = 64;
	int m_ButtonXAdditive = 11;
	int m_AboutHeaderYAdditive = 21;

	SDL_Color m_NormalColor = SDL_Color{ 200, 200, 200, 255 };
	SDL_Color m_HoverColor = SDL_Color{ 100, 100, 100, 255 };
};