#pragma once

#include "glm.hpp"
#include "SDL.h"

#include "managers/InputManager.h"

class Camera
{
public:
	Camera( glm::vec2, glm::vec2 );
	~Camera();

	void Update( float, const InputManager& );
	void RenderImGui();

	glm::vec2 GetPosition() const { return m_Position; };
	glm::vec2 GetScale() const { return m_Scale; };

private:
	glm::vec2 m_Position;
	glm::vec2 m_Scale;
};