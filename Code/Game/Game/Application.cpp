#include "GamePCH.h"

#include "Application.h"

#include <Saz/TransformComponent.h>
#include <Saz/InputComponent.h>
#include <Saz/SpriteComponent.h>
#include <Saz/ResourceManager.h>

#include <SFML/Graphics/Texture.hpp>
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

	// #temporary
	auto& registry = m_EntityWorld.m_Registry;
	if (ImGui::Button("Create Entity with Sprite and Transform"))
	{
		ecs::Entity entity = m_EntityWorld.CreateEntity();
		sf::Texture* pTexture = m_pResourceManager->GetTexture("Ship.png");

		SpriteComponent& spriteComp = m_EntityWorld.AddComponent<SpriteComponent>(entity);
		spriteComp.m_Texture = pTexture;
		TransformComponent& transformComp = m_EntityWorld.AddComponent<TransformComponent>(entity);
		transformComp.m_Position = vec2(600.0f, 360.0f);
		LOG(INFO, "A new entity is created");
	}

	if (ImGui::Button("Add Input"))
	{
		const auto view = registry.view<TransformComponent, SpriteComponent>();
		for (const ecs::Entity& entity : view)
		{
			if (!m_EntityWorld.HasComponent<Input::InputComponent>(entity))
				m_EntityWorld.AddComponent<Input::InputComponent>(entity);

			LOG(WARNING, "Input is given to all entities");
		}
	}

	const auto view = registry.view<TransformComponent, SpriteComponent, Input::InputComponent>();
	for (const ecs::Entity& entity : view)
	{
		const auto& spriteComponent = view.get<SpriteComponent>(entity);
		auto& transformComponent = view.get<TransformComponent>(entity);
		auto& inputComponent = view.get<Input::InputComponent>(entity);
		vec2& pos = transformComponent.m_Position;

		if (inputComponent.IsKeyHeld(Input::EKeyboard::A))
			pos.x -= 0.5f;
		if (inputComponent.IsKeyHeld(Input::EKeyboard::D))
			pos.x += 0.5f;
		if (inputComponent.IsKeyHeld(Input::EKeyboard::W))
			pos.y += 0.5f;
		if (inputComponent.IsKeyHeld(Input::EKeyboard::S))
			pos.y -= 0.5f;
	}

	ImGui::EndFrame();
}

Saz::Application* Saz::CreateApplication()
{
	return new ::Application();
}
