#include <iostream>

#include "imgui.h"
#include "SDL_image.h"

#include "Entity.h"
#include "managers/AudioManager.h"
#include "managers/config.h"

Entity::Entity( EntityType type, EntityDetails details )
	: m_EntityDetails( details ), m_Type( type ), m_DefaultDetails( details )
{

}

Entity::~Entity() {}

void Entity::InitColliders( const TextureManager& textureManager )
{
	m_EntityBounds = EntityBounds( textureManager, m_EntityDetails );
	m_DefaultBounds = m_EntityBounds;
}

void Entity::Update( double deltaTime, const InputManager& inputManager ) {}

void Entity::HandleCollisions( const Entity& entity, std::function<void( int )> updateScoreFunc )
{
	m_CollisionAndOverlap = DetectCollision( entity );

	if( m_CollisionAndOverlap.IsColliding() )
	{
		ResolveCollision( entity );

		if( GetType() == BALL && entity.GetType() == BAT )
		{
			AudioManager::Get().PlaySound( 0, 0 );
			updateScoreFunc( 1 );
		}
	}
}

void Entity::RenderImGui()
{
	if( m_ShowBounds )
	{
		auto min = GetBoundPoint( TOPLEFT );
		auto max = GetBoundPoint( BOTTOMRIGHT );
		auto middle = GetCenter();

		ImGui::GetForegroundDrawList()->AddRect( { min.x, min.y }, { max.x, max.y }, IM_COL32( 255, 0, 0, 255 ), 0, 0, 2.0f );
		ImGui::GetForegroundDrawList()->AddCircleFilled( { GetCenter().x, GetCenter().y }, 3, IM_COL32( 255, 0, 0, 255 ) );
	}


	std::string windowTitle = m_Type == BALL ? "Ball" : "Bat";
	windowTitle += " Settings";

	float midPoint[2] = { GetCenter().x, GetCenter().y };

	ImGui::Begin( windowTitle.c_str() );
	ImGui::Checkbox( "Show Bounds", &m_ShowBounds );
	ImGui::BeginDisabled();
	ImGui::InputFloat2( "Mid Point", midPoint );
	ImGui::EndDisabled();
	ImGui::End();
}

void Entity::Render( SDL_Renderer* renderer, const TextureManager& textureManager ) const
{
	SDL_Rect rect = { GetCenter().x - GetBoundDetails().GetHalfBounds().x, GetCenter().y - GetBoundDetails().GetHalfBounds().y, m_EntityBounds.bounds.x, m_EntityBounds.bounds.y };
	SDL_RenderCopy( renderer, textureManager.GetTexture( m_EntityDetails.textureIndex ).GetTexture(), NULL, &rect );
}

AxisOverlap Entity::DetectCollision( const Entity& entity )
{
	AxisOverlap overlap;

	overlap.xOverlap = !(GetBoundPoint( TOPRIGHT ).x < entity.GetBoundPoint( TOPLEFT ).x) && !(GetBoundPoint( TOPLEFT ).x > entity.GetBoundPoint( TOPRIGHT ).x);
	overlap.yOverlap = !(GetBoundPoint( TOPRIGHT ).y > entity.GetBoundPoint( BOTTOMRIGHT ).y) && !(GetBoundPoint( BOTTOMRIGHT ).y < entity.GetBoundPoint( TOPRIGHT ).y);

	if( overlap.IsColliding() )
	{
		glm::vec2 delta = GetCenter() - entity.GetCenter();

		overlap.overlapAmount = {
			m_EntityBounds.GetHalfBounds().x + entity.m_EntityBounds.GetHalfBounds().x - glm::abs( delta.x ),
			m_EntityBounds.GetHalfBounds().y + entity.m_EntityBounds.GetHalfBounds().y - glm::abs( delta.y )
		};
	}

	return overlap;
}

void Entity::ResolveCollision( const Entity& entity ) {}

void Entity::MaintainBounds() {}

void Entity::ResetDetails()
{
	m_EntityDetails = m_DefaultDetails;
}

void Entity::GroundCheck()
{
	m_IsGrounded = GetBoundPoint( BOTTOMRIGHT ).y >= Config::GetWindowSize().y - Config::GetWindowPadding();
}

std::array<glm::vec2, 4> Entity::GetBoundPoints() const
{
	return
	{
		glm::vec2( m_EntityDetails.pos.x, m_EntityDetails.pos.y ),
		glm::vec2( m_EntityDetails.pos.x + m_EntityBounds.bounds.x, m_EntityDetails.pos.y ),
		glm::vec2( m_EntityDetails.pos.x + m_EntityBounds.bounds.x, m_EntityDetails.pos.y + m_EntityBounds.bounds.y ),
		glm::vec2( m_EntityDetails.pos.x, m_EntityDetails.pos.y + m_EntityBounds.bounds.y )
	};
}

glm::vec2 Entity::GetBoundPoint( BoundPointType type ) const
{
	return glm::vec2( m_EntityDetails.pos.x + (type == TOPRIGHT || type == BOTTOMRIGHT ? 1 : -1) * m_EntityBounds.GetHalfBounds().x, m_EntityDetails.pos.y + (type == BOTTOMLEFT || type == BOTTOMRIGHT ? 1 : -1) * m_EntityBounds.GetHalfBounds().y );
}

glm::vec2 Entity::GetCenter() const
{
	// return glm::vec2( m_EntityDetails.pos + m_EntityBounds.bounds * 0.5f );
	return m_EntityDetails.pos;
}

void Entity::SetBoundPoint( BoundPointType type, glm::vec2 pos )
{
	m_EntityDetails.pos.x = pos.x + (type == TOPLEFT || type == BOTTOMLEFT ? 1 : -1) * GetBoundDetails().GetHalfBounds().x;
	m_EntityDetails.pos.y = pos.y + (type == BOTTOMLEFT || type == BOTTOMRIGHT ? -1 : 1) * GetBoundDetails().GetHalfBounds().y;
}
