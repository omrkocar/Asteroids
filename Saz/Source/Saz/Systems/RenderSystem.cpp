#include "SazPCH.h"
#include "RenderSystem.h"

#include "Saz/Components/RenderComponents.h"
#include "Saz/Components/TransformComponent.h"
#include "Saz/Core/WindowBase.h"
#include "Saz/Vulkan/Model.h"


namespace ecs
{
	RenderSystem::RenderSystem(vulkan::Device& device, vulkan::Renderer& renderer)
		: m_Device(device)
		, m_Renderer(renderer)
	{

	}

	RenderSystem::~RenderSystem()
	{

	}

	void RenderSystem::Destroy()
	{
		auto& registry = m_World->m_Registry;
		const auto view = registry.view<component::TransformComponent, component::SpriteComponent>();
		for (const auto& entity : view)
		{
			component::SpriteComponent& sprite = view.get<component::SpriteComponent>(entity);
			delete sprite.model;
		}
	}

	void RenderSystem::Init()
	{
		const std::vector<vulkan::Vertex> vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<uint32_t> indices = {
			0, 1, 2, 2, 3, 0
		};

		auto model = new vulkan::Model(m_Device, vertices, indices);

		auto entity = m_World->CreateEntity();
		auto& transform = m_World->AddComponent<component::TransformComponent>(entity);
		component::SpriteComponent& spriteComp = m_World->AddComponent<component::SpriteComponent>(entity);
		spriteComp.model = model;
	}

	void RenderSystem::Update(const Saz::GameTime& gameTime)
	{
		if (auto commandBuffer = m_Renderer.BeginFrame())
		{
			auto& registry = m_World->m_Registry;

			m_Renderer.BeginSwapChainRenderPass(commandBuffer);

			const auto view = registry.view<component::TransformComponent, component::SpriteComponent>();
			for (const auto& entity : view)
			{
				component::SpriteComponent& sprite = view.get<component::SpriteComponent>(entity);
				sprite.model->Bind(commandBuffer);
				sprite.model->Draw(commandBuffer);
			}

			m_Renderer.EndFrame();
		}
	}
}
