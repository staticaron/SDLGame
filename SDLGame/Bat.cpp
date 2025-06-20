#include "Bat.h"

#include <iostream>

#include "imgui/imgui.h"

#include "managers/config.h"

Bat::Bat( EntityDetails details )
	: Entity( EntityType::BAT, details )
{
}

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

void Bat::RenderImGui()
{
	Entity::RenderImGui();

	std::string windowTitle = "Bat Settings";

	ImGui::Begin(windowTitle.c_str());;
	ImGui::Checkbox("BOOST Active", &m_BoostActive);
	ImGui::End();

}

void Bat::EnableExpansion()
{
	glm::vec2 newScale = m_DefaultDetails.scale * m_BoostScaleMultiplier;
	glm::vec2 newBounds = m_DefaultBounds.bounds * m_BoostScaleMultiplier;
	m_EntityDetails.scale = newScale;
	m_EntityBounds.bounds = newBounds;
}

void Bat::DisableExpansion()
{
	glm::vec2 newScale = m_DefaultDetails.scale;
	glm::vec2 newBounds = m_DefaultBounds.bounds;
	m_EntityDetails.scale = newScale;
	m_EntityBounds.bounds = newBounds;
}

void Bat::MaintainBounds()
{
	if( GetBoundPoint( TOPLEFT ).x < Config::GetWindowPadding() ) SetBoundPoint( TOPLEFT, { Config::GetWindowPadding(), GetBoundPoint( TOPLEFT ).y } );
	if( GetBoundPoint( BOTTOMRIGHT ).x > Config::GetWindowSize().x - Config::GetWindowPadding() ) SetBoundPoint( BOTTOMRIGHT, { Config::GetWindowSize().x - Config::GetWindowPadding(), GetBoundPoint( BOTTOMRIGHT ).y });
}
