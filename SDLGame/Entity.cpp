#include <iostream>

#include "SDL/SDL_image.h"
#include "Entity.h"
#include "imgui/imgui.h"
#include "managers/config.h"

Entity::Entity( EntityType type, EntityDetails details )
	: m_EntityDetails( details ), m_Type( type )
{

}

Entity::~Entity() {}

void Entity::InitColliders( const TextureManager& textureManager )
{
	m_EntityBounds = EntityBounds( textureManager, m_EntityDetails );
}

void Entity::Update( double deltaTime, const InputManager& inputManager ) {}

bool Entity::HandleCollisions( const Entity& entity, std::function<void(int)> updateScoreFunc)
{
	m_CollisionAndOverlap = DetectCollision( entity );

	if( m_CollisionAndOverlap.IsColliding() && GetType() == BALL )
	{
		if( entity.GetType() == BAT ) updateScoreFunc( 1 );
		ResolveCollision( entity );
	}

	return m_CollisionAndOverlap.isGrounded;
}

void Entity::RenderImGui()
{
	if( m_ShowBounds )
	{
		auto min = m_EntityDetails.pos;
		auto max = m_EntityDetails.pos + m_EntityBounds.bounds;
		auto middle = m_EntityDetails.pos + m_EntityBounds.GetHalfBounds();

		ImGui::GetForegroundDrawList()->AddRect( { min.x, min.y }, { max.x, max.y }, IM_COL32( 255, 0, 0, 255 ), 0, 0, 2.0f );
		ImGui::GetForegroundDrawList()->AddCircleFilled( { GetCenter().x, GetCenter().y }, 3, IM_COL32( 255, 0, 0, 255 ) );
	}

	float maxArr[2] = { GetCenter().x, GetCenter().y };

	std::string windowTitle = m_Type == BALL ? "Ball" : "Bat";
	windowTitle += " Settings";

	ImGui::Begin( windowTitle.c_str() );
	ImGui::InputFloat2( "Max Point", maxArr );
	ImGui::Checkbox( "Show Bounds", &m_ShowBounds );
	ImGui::End();
}

void Entity::Render( SDL_Renderer* renderer, const TextureManager& textureManager ) const
{
	SDL_Rect rect = { m_EntityDetails.pos.x, m_EntityDetails.pos.y, m_EntityBounds.bounds.x, m_EntityBounds.bounds.y };
	SDL_RenderCopy( renderer, textureManager.GetTexture( m_EntityDetails.textureIndex ).texture, NULL, &rect );
}

AxisOverlap Entity::DetectCollision( const Entity& entity ) const
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

	overlap.isGrounded = GetBoundPoint(BOTTOMRIGHT).y >= Config::GetWindowSize().y - Config::GetWindowPadding();

	return overlap;
}

void Entity::ResolveCollision( const Entity& entity ) {}

void Entity::MaintainBounds() {}

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
	return glm::vec2( m_EntityDetails.pos.x + (type == TOPRIGHT || type == BOTTOMRIGHT ? m_EntityBounds.bounds.x : 0), m_EntityDetails.pos.y + (type == BOTTOMRIGHT || type == BOTTOMLEFT ? m_EntityBounds.bounds.y : 0) );
}

glm::vec2 Entity::GetCenter() const
{
	return glm::vec2( m_EntityDetails.pos + m_EntityBounds.bounds * 0.5f );
}
