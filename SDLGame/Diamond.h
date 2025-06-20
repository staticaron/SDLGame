#pragma once

#include <functional>

#include "Entity.h"

class Diamond : public Entity
{
	public:
	Diamond( EntityDetails );

	void ResetDiamond();
	void HandleCollisions( const Entity&, std::function<void( int )> ) override;
	private:
};