#include "SazPCH.h"

#include "Saz/CameraComponent.h"
#include "Saz/RenderSystem.h"
#include "Saz/RenderComponents.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"

#include "Saz/Vulkan/Pipeline.h"
#include "Saz/Vulkan/Renderer.h"
#include "Saz/Vulkan/Model.h"
#include "Saz/Vulkan/Device.h"
#include "Saz/GLFW/Window.h"

#include <entt/entt.hpp>


namespace 
{
	struct SimplePushConstantData
	{
		Matrix transform = Matrix::Identity;
		vec2 offset;
		alignas(16) vec3 color;
	};
}
namespace ecs
{	
	RenderSystem::RenderSystem(vulkan::Device& device, Saz::glfw::Window& window)
		: m_Device(device)
		, m_GLFWWindow(window)
	{
		m_Renderer = new vulkan::Renderer(m_GLFWWindow, m_Device);

		CreatePipelineLayout();
		CreatePipeline();
	}

	RenderSystem::~RenderSystem()
	{
		delete m_Renderer;
		vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
	}

	void RenderSystem::PostInit()
	{
		LoadObjects();
	}

	void RenderSystem::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;

		if (auto commandBuffer = m_Renderer->BeginFrame())
		{
			m_Pipeline->Bind(commandBuffer);

			m_Renderer->BeginSwapChainRenderPass(commandBuffer);
			
			const auto view = registry.view<component::TransformComponent, component::RenderComponent>();
			for (const auto& entity : view)
			{
				component::TransformComponent& transformComponent = view.get<component::TransformComponent>(entity);
				component::RenderComponent& renderComponent = view.get<component::RenderComponent>(entity);

				SimplePushConstantData push{};
				push.offset = vec2(transformComponent.m_Position.x, transformComponent.m_Position.y);
				push.color = renderComponent.color;

				transformComponent.m_Rotation += Rotator(0.001f);
				
				vkCmdPushConstants(commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
				renderComponent.model->Bind(commandBuffer);
				renderComponent.model->Draw(commandBuffer);
			}

			m_Renderer->EndSwapChainRenderPass(commandBuffer);
			m_Renderer->EndFrame();
		}
	}

	void RenderSystem::LoadObjects()
	{
		DynamicArray<vulkan::Vertex> vertices
		{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		};

		auto model = std::make_shared<vulkan::Model>(m_Device, vertices);

		auto& registry = m_World->m_Registry;
		const auto view = registry.view<component::TransformComponent, component::RenderComponent>();
		for (const auto& entity : view)
		{
			component::RenderComponent& renderComponent = view.get<component::RenderComponent>(entity);

			renderComponent.model = model;
		}
	}

	void RenderSystem::CreatePipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		bool success = vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) == VK_SUCCESS;
		SAZ_ASSERT(success, "Failed to create Pipeline Layout!");
	}

	void RenderSystem::CreatePipeline()
	{
		SAZ_ASSERT(m_PipelineLayout != nullptr, "Cannot create pipeline before pipeline layout!");

		vulkan::PipelineConfig pipelineConfig{};
		vulkan::Pipeline::DefaultPipelineConfig(pipelineConfig);

		pipelineConfig.renderPass = m_Renderer->GetSwapChainRenderPass();
		pipelineConfig.pipelineLayout = m_PipelineLayout;
		m_Pipeline = std::make_unique<vulkan::Pipeline>(m_Device, pipelineConfig);
	}
}
