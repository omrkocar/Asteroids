#include "GamePCH.h"

#include "Application.h"

#include "Game/WorldOutliner.h"
#include "Game/Inspector.h"

#include <Saz/TransformComponent.h>
#include <Saz/InputComponent.h>
#include <Saz/RenderComponents.h>
#include <Saz/ResourceManager.h>
#include <Saz/NameComponent.h>
#include <Saz/LevelSystem.h>
#include <Saz/GameTime.h>

#include <entt/entt.hpp>
#include "Saz/LevelComponent.h"

#include <imgui/imgui.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	return projection;
}

class ExampleLayer : public Saz::Layer
{
public:
	ExampleLayer()
		: Layer("Placeholder")
	{
		
	}

	void OnUpdate(const Saz::GameTime& ts)
	{
		
	}

	void OnEvent(Saz::Event& event)
	{
		
	}
};


Application::Application()
{
	PushLayer(new ExampleLayer());
}

Application::~Application()
{
	Destroy();
}

void Application::Init()
{
	Saz::Application::Init();

	m_EntityWorld.RegisterSystem<ecs::WorldOutliner>();
	m_EntityWorld.RegisterSystem<ecs::Inspector>(m_EntityWorld.GetSystem<ecs::WorldOutliner>());

	ecs::LevelSystem& levelSystem = m_EntityWorld.GetSystem<ecs::LevelSystem>();
	levelSystem.LoadFromFile("DefaultScene.scene");
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
}

void Application::DrawMenuBar()
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Level"))
	{
		if (ImGui::MenuItem("Clear Level"))
		{
			m_EntityWorld.DestroyAllEntities();
			m_EntityWorld.GetSystem<ecs::WorldOutliner>().m_IsObjectInspectorOn = false;
		}

		if (ImGui::MenuItem("Load Default Level"))
		{
			ecs::LevelSystem& levelSystem = m_EntityWorld.GetSystem<ecs::LevelSystem>();
			levelSystem.LoadFromFile("DefaultScene.scene");
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

			component::RenderComponent& renderComp = m_EntityWorld.AddComponent<component::RenderComponent>(entity);
			m_EntityWorld.AddComponent<component::LevelComponent>(entity);
			auto& nameComp = m_EntityWorld.AddComponent<component::NameComponent>(entity);
			nameComp.m_Name = "Object(" + std::to_string(index) +  ")";
			index++;
			component::TransformComponent& transformComp = m_EntityWorld.AddComponent<component::TransformComponent>(entity);
			transformComp.m_Position = vec2(Random::Range(0.0f, 1000.0f), Random::Range(0.0f, 500.0f));
			//IMGUI_LOG_INFO("A new entity is created at %f, %f", transformComp.m_Position.x, transformComp.m_Position.y);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();
}

Saz::Application* Saz::CreateApplication()
{
	return new ::Application();
}
