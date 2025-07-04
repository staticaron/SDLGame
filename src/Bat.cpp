#include "Bat.h"

#include <iostream>

#include "imgui.h"

#include "managers/config.h"

Bat::Bat( EntityDetails details ) : Entity( EntityType::BAT, details ) {}

void Bat::Update( double deltaTime, const InputManager& inputManager )
{
	if( m_IsStatic ) return;

	GroundCheck();

	m_PreviousEntityDetails = m_EntityDetails;
	m_EntityDetails.pos.x += inputManager.m_HorizontalAxis * m_EntityDetails.moveSpeed * deltaTime;

	if( inputManager.m_Space )
	{
		if( IsBoostActive() )
		{
			DisableBoost();
		}
		else
		{
			EnableBoost();
		}
	}

	MaintainBounds();
}

void Bat::Render( SDL_Renderer* renderer, const TextureManager& textureManager ) const
{
	SDL_Rect rect = { GetCenter().x - GetBoundDetails().GetHalfBounds().x, GetCenter().y - GetBoundDetails().GetHalfBounds().y, m_EntityBounds.bounds.x, m_EntityBounds.bounds.y };

	const auto& tex = m_Expanded == false ? textureManager.GetTexture( m_EntityDetails.textureIndex ).GetTexture() : textureManager.GetTexture( m_EntityDetails.textureIndex + 1 ).GetTexture();

	if( m_BoostActive ) SDL_SetTextureColorMod( tex, 150, 150, 150 );
	else SDL_SetTextureColorMod( tex, 255, 255, 255 );

	SDL_RenderCopy( renderer, tex, NULL, &rect );
}

void Bat::RenderImGui()
{
	std::string windowTitle = "Bat Settings";

	ImGui::Begin( windowTitle.c_str() );;
	ImGui::Checkbox( "BOOST Active", &m_BoostActive );
	ImGui::End();

}

void Bat::ResetDetails()
{
	Entity::ResetDetails();

	if( m_Expanded ) DisableExpansion();
}

void Bat::EnableExpansion()
{
	glm::vec2 newScale = m_DefaultDetails.scale * m_BoostScaleMultiplier;
	glm::vec2 newBounds = m_DefaultBounds.bounds * m_BoostScaleMultiplier;
	m_EntityDetails.scale = newScale;
	m_EntityBounds.bounds = newBounds;

	m_Expanded = true;
}

void Bat::DisableExpansion()
{
	glm::vec2 newScale = m_DefaultDetails.scale;
	glm::vec2 newBounds = m_DefaultBounds.bounds;
	m_EntityDetails.scale = newScale;
	m_EntityBounds.bounds = newBounds;

	m_Expanded = false;
}

void Bat::MaintainBounds()
{
	if( GetBoundPoint( BoundPointType::TOPLEFT ).x < Config::GetWindowPadding() ) SetBoundPoint( BoundPointType::TOPLEFT, { Config::GetWindowPadding(), GetBoundPoint( BoundPointType::TOPLEFT ).y } );
	if( GetBoundPoint( BoundPointType::BOTTOMRIGHT ).x > Config::GetWindowSize().x - Config::GetWindowPadding() ) SetBoundPoint( BoundPointType::BOTTOMRIGHT, { Config::GetWindowSize().x - Config::GetWindowPadding(), GetBoundPoint( BoundPointType::BOTTOMRIGHT ).y } );
}
