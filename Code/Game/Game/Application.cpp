#include "GamePCH.h"

#include "Application.h"

#include <Saz/TransformComponent.h>
#include <Saz/InputComponent.h>
#include <Saz/RenderComponents.h>
#include <Saz/ResourceManager.h>
#include <Saz/NameComponent.h>
#include <Saz/LevelSystem.h>
#include <Saz/SFML/Window.h>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

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

	m_pResourceManager->LoadTexture("Ship.png");
	m_pResourceManager->LoadTexture("Hollow_Knight.png");
	m_pResourceManager->LoadTexture("Island.png");
	m_pResourceManager->LoadTexture("Water.png");

	m_pResourceManager->CreateRectangle("Plane", vec2(100.0f, 10.0f), sf::Color::Blue);

	// #todo: Fix the paths asap
	ecs::LevelSystem& levelSystem = m_EntityWorld.GetSystem<ecs::LevelSystem>();
	levelSystem.LoadFromFile("Scenes/DefaultScene.scene");

	auto entity = m_EntityWorld.CreateEntity();
	sf::RectangleShape* plane = m_pResourceManager->CreateRectangle("Plane", vec2(100.0f, 10.0f), sf::Color::Blue);
	component::RenderComponent& renderComp = m_EntityWorld.AddComponent<component::RenderComponent>(entity);

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

void Application::Update()
{
	Saz::Application::Update();

	DrawMenuBar();

	ImGui::EndFrame();
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
			levelSystem.LoadFromFile("Scenes/DefaultScene.scene");
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
