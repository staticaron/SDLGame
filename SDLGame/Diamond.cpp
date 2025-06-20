#include "Diamond.h"

#include "glm/glm.hpp"
#include "glm/gtc/random.hpp"

Diamond::Diamond( EntityDetails details ) : Entity( EntityType::DIAMOND, details) {}

void Diamond::ResetDiamond()
{
	m_EntityDetails.pos = { glm::linearRand( 100, 700), glm::linearRand( 100, 150) };
}

void Diamond::HandleCollisions( const Entity& entity, std::function<void( int )> updateScoreFunc )
{
	auto collisionDetails = DetectCollision( entity );

	if( entity.GetType() == BALL && collisionDetails.IsColliding() )
	{
		ResetDiamond();
		updateScoreFunc( 10 );
	}
}
