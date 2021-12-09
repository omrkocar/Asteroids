#include "GamePCH.h"

#include "Application.h"

#include "WorldOutliner.h"
#include "SceneEditor.h"

#include <Saz/TransformComponent.h>
#include <Saz/InputComponent.h>
#include <Saz/RenderComponents.h>
#include <Saz/ResourceManager.h>
#include <Saz/NameComponent.h>
#include <Saz/LevelSystem.h>
#include <Saz/GameTime.h>

#include <entt/entt.hpp>


Application::Application()
{
	
}

Application::~Application()
{
	Destroy();
}

void Application::Init()
{
	Saz::Application::Init();

	m_EntityWorld.RegisterSystem<ecs::SceneEditor>();

	ecs::LevelSystem& levelSystem = m_EntityWorld.GetSystem<ecs::LevelSystem>();
	levelSystem.LoadFromFile("DefaultScene.scene");

	IMGUI_LOG_INFO("Registry Size: %d", m_EntityWorld.m_Registry.size());
}

void Application::Destroy()
{
	Saz::Application::Destroy();
}
 
void Application::Register()
{
	Saz::Application::Register();
}

void Application::Update(const Saz::GameTime& gameTime)
{
	Saz::Application::Update(gameTime);

	auto& registry = m_EntityWorld.m_Registry;
	const auto view = registry.view<component::TransformComponent, component::InputComponent>();
	for (const ecs::Entity& entity : view)
	{
		component::TransformComponent& transformComp = view.get<component::TransformComponent>(entity);
		component::InputComponent& inputComponent = view.get<component::InputComponent>(entity);
		if (inputComponent.IsKeyHeld(Input::EKeyboard::A))
		{
			transformComp.m_Position.x -= 500.0f * gameTime.m_DeltaTime;
		}
		if (inputComponent.IsKeyHeld(Input::EKeyboard::D))
		{
			transformComp.m_Position.x += 500.0f * gameTime.m_DeltaTime;
		}
		if (inputComponent.IsKeyHeld(Input::EKeyboard::W))
		{
			transformComp.m_Position.y -= 500.0f * gameTime.m_DeltaTime;
		}
		if (inputComponent.IsKeyHeld(Input::EKeyboard::S))
		{
			transformComp.m_Position.y += 500.0f * gameTime.m_DeltaTime;
		}
	}

	DrawMenuBar();
}

void Application::DrawMenuBar()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Level"))
	{
		if (ImGui::MenuItem("Clear Level"))
		{
			m_EntityWorld.DestroyAllEntities();
			IMGUI_LOG_INFO("Registry Size: %d", m_EntityWorld.m_Registry.size());
		}

		if (ImGui::MenuItem("Load Default Level"))
		{
			ecs::LevelSystem& levelSystem = m_EntityWorld.GetSystem<ecs::LevelSystem>();
			levelSystem.LoadFromFile("DefaultScene.scene");
			IMGUI_LOG_INFO("Registry Size: %d", m_EntityWorld.m_Registry.size());
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

Saz::Application* Saz::CreateApplication()
{
	return new ::Application();
}
