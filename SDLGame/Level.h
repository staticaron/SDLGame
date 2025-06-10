#pragma once

#include <SDL/SDL.h>
#include <vector>

#include "Entity.h"
#include "managers/TextureManager.h"
#include "managers/InputManager.h"
#include "managers/EntityManager.h"

class Level 
{
	public:
	Level();
	~Level();

	void InitColliders(const TextureManager&);

	void HandleCollisions();
	void Update(double, const InputManager&);
	void RenderImGui();
	void Render(SDL_Renderer*, const TextureManager&);

	private:
	EntityManager m_EntityManager;
};