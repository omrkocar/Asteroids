#include "EditorPCH.h"

#include "Application.h"

#include "Editor/WorldOutliner.h"
#include "Editor/Inspector.h"
#include "SceneEditor.h"
#include "ContentBrowser.h"

#include "Saz/CameraComponent.h"
#include "Saz/Core/EntityWorld.h"
#include "Saz/Core/GameTime.h"
#include "Saz/InputComponent.h"
#include "Saz/LevelComponent.h"
#include "Saz/MovementComponent.h"
#include "Saz/NameComponent.h"
#include "Saz/PhysicsComponents.h"
#include "Saz/RenderComponents.h"
#include "Saz/SceneComponent.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"
#include "Saz/Systems/CameraSystem.h"
#include "Saz/Systems/InputSystem.h"
#include "Saz/Systems/PhysicsSystem.h"
#include "Saz/Systems/RenderSystem.h"
#include "Saz/Systems/SceneSystem.h"
#include "MenuBar.h"
#include "ProjectSettingsWindow.h"
#include "Saz/WindowResizedOneFrameComponent.h"

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

	m_World.RegisterComponent<component::EditorCameraComponent>();
	m_World.RegisterComponent<component::InputComponent>();
	m_World.RegisterComponent<component::MovementComponent>();
	m_World.RegisterComponent<component::NameComponent>();
	m_World.RegisterComponent<component::SpriteComponent>();
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
	m_World.RegisterSystem<ecs::CameraSystem>();
	m_World.RegisterSystem<ecs::SceneSystem>();
	m_World.RegisterSystem<ecs::WorldOutliner>();
	m_World.RegisterSystem<ecs::ProjectSettingsWindow>();
	m_World.RegisterSystem<ecs::MenuBar>(m_World.GetSystem<ecs::ProjectSettingsWindow>());
	m_World.RegisterSystem<ecs::SceneEditor>(m_World.GetSystem<ecs::WorldOutliner>());
	m_World.RegisterSystem<ecs::PhysicsSystem>();
	m_World.RegisterSystem<ecs::RenderSystem>(*m_Window, m_World.GetSystem<ecs::CameraSystem>());

	m_World.RegisterSystem<ecs::Inspector>(m_World.GetSystem<ecs::WorldOutliner>());
	m_World.RegisterSystem<ecs::ContentBrowser>();
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
