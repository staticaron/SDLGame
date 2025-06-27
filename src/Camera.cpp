#include "Camera.h"

#include "imgui.h"

Camera::Camera( glm::vec2 position, glm::vec2 scale ) : m_Position( position ), m_Scale( scale ) {}

Camera::~Camera() {}

void Camera::Update( float deltaTime, const InputManager& inputManager )
{

}

void Camera::RenderImGui()
{
	ImGui::Begin( "Camera Settings" );
	float pos[2] = { m_Position.x, m_Position.y };
	if( ImGui::DragFloat2( "Position", pos ) ) m_Position = { pos[0], pos[1] };

	float scale[2] = { m_Scale.x, m_Scale.y };
	if( ImGui::DragFloat2( "Scale", scale ) ) m_Scale = { scale[0], scale[1] };
	ImGui::End();
}
