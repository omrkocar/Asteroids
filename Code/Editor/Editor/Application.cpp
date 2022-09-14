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
}

void Application::Destroy()
{
	Saz::Application::Destroy();
}
 
void Application::Register()
{
	Saz::Application::Register();

	m_EntityWorld.RegisterComponent<component::EditorCameraComponent>();
	m_EntityWorld.RegisterComponent<component::InputComponent>();
	m_EntityWorld.RegisterComponent<component::MovementComponent>();
	m_EntityWorld.RegisterComponent<component::NameComponent>();
	m_EntityWorld.RegisterComponent<component::SpriteComponent>();
	m_EntityWorld.RegisterComponent<component::Rigidbody2DComponent>();
	m_EntityWorld.RegisterComponent<component::BoxCollider2DComponent>();
	m_EntityWorld.RegisterComponent<component::TransformComponent>();
	m_EntityWorld.RegisterComponent<component::LoadedSceneComponent>();
	m_EntityWorld.RegisterComponent<component::SceneEntityComponent>();
	m_EntityWorld.RegisterComponent<component::LoadSceneRequestOneFrameComponent>();
	m_EntityWorld.RegisterComponent<component::SaveSceneRequestOneFrameComponent>();
	m_EntityWorld.RegisterComponent<component::NewSceneRequestOneFrameComponent>();
	m_EntityWorld.RegisterComponent<component::SceneStateChangedOneFrameComponent>();

	m_EntityWorld.RegisterSystem<ecs::InputSystem>(*m_Window);
	m_EntityWorld.RegisterSystem<ecs::CameraSystem>();
	m_EntityWorld.RegisterSystem<ecs::WorldOutliner>();
	m_EntityWorld.RegisterSystem<ecs::SceneSystem>();
	m_EntityWorld.RegisterSystem<ecs::SceneEditor>(m_EntityWorld.GetSystem<ecs::WorldOutliner>());
	m_EntityWorld.RegisterSystem<ecs::PhysicsSystem>();
	m_EntityWorld.RegisterSystem<ecs::RenderSystem>(*m_Window, m_EntityWorld.GetSystem<ecs::CameraSystem>());

	m_EntityWorld.RegisterSystem<ecs::Inspector>(m_EntityWorld.GetSystem<ecs::WorldOutliner>());
	m_EntityWorld.RegisterSystem<ecs::ContentBrowser>();
}

void Application::Update(const Saz::GameTime& gameTime)
{
	Saz::Application::Update(gameTime);
}

Saz::Application* Saz::CreateApplication()
{
	return new ::Application();
}
