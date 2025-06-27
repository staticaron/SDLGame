#include "Entity.h"

#include "managers/EntityManager.h"

EntityManager::EntityManager( EntityDetails batDetails, EntityDetails ballDetails, EntityDetails diamondDetails )
	: m_Bat( batDetails ), m_Ball( ballDetails ), m_Diamond( diamondDetails )
{
}

std::vector<std::reference_wrapper<Entity>> EntityManager::GetEntities()
{
	return { std::ref( m_Ball ), std::ref( m_Bat ), std::ref( m_Diamond ) };
}