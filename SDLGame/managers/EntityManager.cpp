#include "../Entity.h"
#include "EntityManager.h"

EntityManager::EntityManager( EntityDetails batDetails, EntityDetails ballDetails )
	: m_Bat( batDetails ), m_Ball( ballDetails )
{
}

std::vector<std::reference_wrapper<Entity>> EntityManager::GetEntities()
{
	return { std::ref( m_Ball ), std::ref( m_Bat ) };
}