#include "GamePCH.h"

#include "Application.h"

#include "Game/WorldOutliner.h"
#include "Game/SceneEditor.h"

#include <Saz/TransformComponent.h>
#include <Saz/InputComponent.h>
#include <Saz/RenderComponents.h>
#include <Saz/ResourceManager.h>
#include <Saz/NameComponent.h>
#include <Saz/LevelSystem.h>
#include <Saz/GameTime.h>

#include <entt/entt.hpp>
#include "Saz/LevelComponent.h"


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
	m_EntityWorld.RegisterSystem<ecs::WorldOutliner>();

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
	//ImGui::ShowDemoWindow();
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

	if (ImGui::BeginMenu("Debug"))
	{
		auto& registry = m_EntityWorld.m_Registry;
		if (ImGui::MenuItem("Create Entity"))
		{
			static int index = 0;
			ecs::Entity entity = m_EntityWorld.CreateEntity();
			raylib::Texture* pTexture = new raylib::Texture("D:/Dev/Saz/Data/Textures/Ship.png");

			component::RenderComponent& renderComp = m_EntityWorld.AddComponent<component::RenderComponent>(entity);
			renderComp.texture = pTexture;
			m_EntityWorld.AddComponent<component::LevelComponent>(entity);
			auto& nameComp = m_EntityWorld.AddComponent<component::NameComponent>(entity);
			nameComp.m_Name = "Object(" + std::to_string(index) +  ")";
			index++;
			component::TransformComponent& transformComp = m_EntityWorld.AddComponent<component::TransformComponent>(entity);
			transformComp.m_Position = vec2(Random::Range(0.0f, 1000.0f), Random::Range(0.0f, 500.0f));
			IMGUI_LOG_INFO("A new entity is created at %f, %f", transformComp.m_Position.x, transformComp.m_Position.y);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

Saz::Application* Saz::CreateApplication()
{
	return new ::Application();
}
