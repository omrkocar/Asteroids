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
#include "GameTime.h"


namespace 
{
	struct SimplePushConstantData
	{
		Matrix transform = Matrix::Identity;
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

			const auto cameraView = registry.view<component::CameraComponent, component::TransformComponent>();
			for (const auto& entity : cameraView)
			{
					component::CameraComponent& cameraComponent = cameraView.get<component::CameraComponent>(entity);
				{
					component::TransformComponent& transformComponent = cameraView.get<component::TransformComponent>(entity);

					if (cameraComponent.m_ViewType == ViewType::Orthographic)
					{
						cameraComponent.m_ProjectionMatrix.CreateOrtho(-transformComponent.m_Scale.x, transformComponent.m_Scale.x,
							-transformComponent.m_Scale.y, transformComponent.m_Scale.y,
							-1000, 1000);
					}
					else
					{
						cameraComponent.m_ViewMatrix.CreateSRT(vec3(1.0f), Quaternion::FromRotator(transformComponent.m_Rotation), transformComponent.m_Position);
						cameraComponent.m_ViewMatrix.Inverse();
						cameraComponent.m_ProjectionMatrix.CreatePerspectiveVFoV(60.0f, Screen::width / Screen::height, 0.01f, 1000.0f);
					}
				}

				const auto view = registry.view<component::TransformComponent, component::RenderComponent>();
				for (const auto& entity : view)
				{
					component::TransformComponent& transformComponent = view.get<component::TransformComponent>(entity);
					component::RenderComponent& renderComponent = view.get<component::RenderComponent>(entity);

					transformComponent.m_Rotation.x = std::fmod(Math::ToDegrees(gameTime.m_TotalTime) * 0.5f, 360.0f);
					transformComponent.m_Rotation.y = std::fmod(Math::ToDegrees(gameTime.m_TotalTime) * 2.0f, 360.0f);

					Matrix transform;
					transform.CreateSRT(transformComponent.m_Scale, Quaternion::FromRotator(transformComponent.m_Rotation), transformComponent.m_Position);

					SimplePushConstantData push{};
					push.transform = cameraComponent.m_ProjectionMatrix * cameraComponent.m_ViewMatrix * transform;
					push.color = renderComponent.color;

					vkCmdPushConstants(commandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
					renderComponent.model->Bind(commandBuffer);
					renderComponent.model->Draw(commandBuffer);
				}
			}

			m_Renderer->EndSwapChainRenderPass(commandBuffer);
			m_Renderer->EndFrame();
		}
	}

	std::unique_ptr<vulkan::Model> RenderSystem::CreateCube(vulkan::Device& device, vec3 offset)
	{
		DynamicArray<vulkan::Vertex> vertices{

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},

			// nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

		};
		for (auto& v : vertices) {
			v.position += offset;
		}
		return std::make_unique<vulkan::Model>(device, vertices);
	}

	void RenderSystem::LoadObjects()
	{
		std::shared_ptr<vulkan::Model> model = CreateCube(m_Device, { 0.0f, 0.0f, 0.0f });

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
