#include "EditorPCH.h"
#include "EditorLayer.h"
#include "Saz/Platform/OpenGL/OpenGLShader.h"
#include "Saz/Rendering/RenderCommand.h"
#include "Saz/Rendering/Renderer.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


static const uint32_t s_MapWidth = 24;

EditorLayer::EditorLayer()
	: Layer("EditorLayer")
{

}

void EditorLayer::OnAttach()
{
	m_Texture = Saz::Texture2D::Create("C:/Dev/SazEngine/Data/Textures/Checkerboard.png");
}

void EditorLayer::OnUpdate(const Saz::GameTime& gameTime)
{
	SAZ_PROFILE_FUNCTION();
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnImGuiRender()
{
}
