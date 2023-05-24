#include "EditorPCH.h"

#include "Application.h"

#include "Editor/WorldOutliner.h"
#include "Editor/ProjectSettingsWindow.h"

#include <Saz/Components/InputComponent.h>
#include <Saz/Components/LevelComponent.h>
#include <Saz/Components/MovementComponent.h>
#include <Saz/Components/NameComponent.h>
#include <Saz/Components/PhysicsComponents.h>
#include <Saz/Components/RenderComponents.h>
#include <Saz/Components/SceneComponent.h>
#include <Saz/Components/TransformComponent.h>
#include <Saz/Components/WindowResizedOneFrameComponent.h>
#include <Saz/Core/EntityWorld.h>
#include <Saz/Core/GameTime.h>
#include <Saz/Screen.h>
#include <Saz/Systems/InputSystem.h>
#include <Saz/Systems/RenderSystem.h>
#include <Saz/Systems/SceneSystem.h>

Application::Application()
	: Saz::Application("Saz Editor")
{
	
}

Application::~Application()
{
	Destroy();
}

void Application::Init()
{
	Saz::Application::Init();

	m_World.Init();
}

void Application::Destroy()
{
	Saz::Application::Destroy();

	m_World.Destroy();
}
 
void Application::Register()
{
	Saz::Application::Register();

	m_World.RegisterComponent<component::InputComponent>();
	m_World.RegisterComponent<component::MovementComponent>();
	m_World.RegisterComponent<component::NameComponent>();
	m_World.RegisterComponent<component::SpriteComponent>();
	m_World.RegisterComponent<component::CircleRendererComponent>();
	m_World.RegisterComponent<component::Rigidbody2DComponent>();
	m_World.RegisterComponent<component::BoxCollider2DComponent>();
	m_World.RegisterComponent<component::TransformComponent>();
	m_World.RegisterComponent<component::LoadedSceneComponent>();
	m_World.RegisterComponent<component::SceneEntityComponent>();
	m_World.RegisterComponent<component::LoadSceneRequestOneFrameComponent>();
	m_World.RegisterComponent<component::SaveSceneRequestOneFrameComponent>();
	m_World.RegisterComponent<component::NewSceneRequestOneFrameComponent>();
	m_World.RegisterComponent<component::SceneStateChangeRequestOneFrameComponent>();
	m_World.RegisterComponent<component::WindowResizedOneFrameComponent>();

	m_World.RegisterSystem<ecs::InputSystem>(*m_Window);
	m_World.RegisterSystem<ecs::SceneSystem>();
	m_World.RegisterSystem<ecs::WorldOutliner>();
	m_World.RegisterSystem<ecs::ProjectSettingsWindow>();
	m_World.RegisterSystem<ecs::RenderSystem>();

}

void Application::Update(const Saz::GameTime& gameTime)
{
	Saz::Application::Update(gameTime);

	if (!m_Minimized)
	{
		m_ImGuiLayer->Begin();
		m_World.Update(gameTime);
		m_ImGuiLayer->End();
	}
}

Saz::Application* Saz::CreateApplication()
{
	return new ::Application();
}
