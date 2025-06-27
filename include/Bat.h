#pragma once

#include "SDL.h"

#include "Entity.h"

class Bat : public Entity
{
public:
	Bat( EntityDetails details );

	void Update( double, const InputManager& inputManager ) override;

	void Render( SDL_Renderer* renderer, const TextureManager& ) const override;
	void RenderImGui() override;

	void ResetDetails() override;

	bool IsBoostActive() const { return m_BoostActive; };
	void EnableBoost() { m_BoostActive = true; };
	void DisableBoost() { m_BoostActive = false; };

	void EnableExpansion();
	void DisableExpansion();

	float GetBoostForceAmount() const { return m_BoostForce; };

private:
	int m_PaddingX = 10;

	glm::vec2 m_BoostScaleMultiplier = { 2, 1 };

	float m_BoostForce = 2.0f;
	bool m_BoostActive = false;
	bool m_Expanded = false;

	void MaintainBounds() override;
};