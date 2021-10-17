#include "SazPCH.h"
#include "Application.h"

#include "Saz/CameraComponent.h"
#include "Saz/EntityWorld.h"
#include "Saz/GameTime.h"
#include "Saz/InputComponent.h"
#include "Saz/InputSystem.h"
#include "Saz/RenderSystem.h"
#include "Saz/ResourceManager.h"
#include "Saz/GLFW/Window.h"
#include "Saz/SFML/Window.h"
#include "Saz/LevelComponent.h"
#include "Saz/LevelSystem.h"
#include "Saz/MovementComponent.h"
#include "Saz/RenderComponents.h"
#include "Saz/NameComponent.h"
#include "Saz/Screen.h"
#include "Saz/TransformComponent.h"
#include "Saz/TransformSystem.h"
#include "Saz/Vulkan/Pipeline.h"
#include "Saz/Vulkan/Device.h"
#include "Saz/Vulkan/SwapChain.h"
#include "Saz/Vulkan/Model.h"


#include <GLFW/glfw3.h>

namespace
{
	void Triangles(
		std::vector<vulkan::Vertex>& vertices,
		int depth,
		vec2 left,
		vec2 right,
		vec2 top) 
	{
		if (depth <= 0) {
			vertices.push_back({ top });
			vertices.push_back({ right });
			vertices.push_back({ left });
		}
		else {
			auto leftTop = 0.5f * (left + top);
			auto rightTop = 0.5f * (right + top);
			auto leftRight = 0.5f * (left + right);
			Triangles(vertices, depth - 1, left, leftRight, leftTop);
			Triangles(vertices, depth - 1, leftRight, right, rightTop);
			Triangles(vertices, depth - 1, leftTop, rightTop, top);
		}
	}
}

namespace Saz
{
	struct SimplePushConstantData
	{
		Matrix transform = Matrix::Identity;
		vec2 offset;
		alignas(16) vec3 color;
	};

	Application* Application::s_Instance = nullptr;

	const ecs::EntityWorld& Application::GetWorld()
	{
		return m_EntityWorld;
	}

	Application::Application()
	{
		spd::Log::Init();

		SAZ_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		
		m_ImGuiLog = new imgui::Log();

		// #todo Create all textures with a single call here.
		m_pResourceManager = new Saz::ResourceManager();
		
		/*{
			Saz::WindowProps windowProps ;
			windowProps.m_Title = "SFML Window";
			windowProps.m_Size = { static_cast<int>(Screen::width), static_cast<int>(Screen::height) };
			m_SFMLWindow = new sfml::Window(windowProps);
		}*/

		{
			glfwInit();

			Saz::WindowProps windowProps;
			windowProps.m_Title = "GLFW Window";
			windowProps.m_Size = { static_cast<int>(Screen::width), static_cast<int>(Screen::height) };
			m_GLFWWindow = new glfw::Window(windowProps);

			m_Device = std::make_unique<vulkan::Device>(*m_GLFWWindow);

			LoadModels();
			CreatePipelineLayout();
			RecreateSwapChain();
			CreateCommandBuffers();
		}
	}

	Application::~Application()
	{
		//delete m_SFMLWindow;
		delete m_GLFWWindow;
		delete m_ImGuiLog;
		delete m_pResourceManager;
		vkDestroyPipelineLayout(m_Device->device(), m_PipelineLayout, nullptr);
		glfwTerminate();
	}

	void Application::Init()
	{
		//m_SFMLWindow->Init();
		m_EntityWorld.Init();
	}

	void Application::Register()
	{
		m_EntityWorld.RegisterComponent<component::InputComponent>();
		m_EntityWorld.RegisterComponent<component::LevelComponent>();
		m_EntityWorld.RegisterComponent<component::MovementComponent>();
		m_EntityWorld.RegisterComponent<component::NameComponent>();
		m_EntityWorld.RegisterComponent<component::RenderComponent>();
		m_EntityWorld.RegisterComponent<component::TransformComponent>();
		m_EntityWorld.RegisterComponent<component::CameraComponent>();

		m_EntityWorld.RegisterSystem<ecs::InputSystem>(*m_SFMLWindow);
		m_EntityWorld.RegisterSystem<ecs::LevelSystem>(*m_pResourceManager);
		m_EntityWorld.RegisterSystem<ecs::RenderSystem>(*m_SFMLWindow);
		m_EntityWorld.RegisterSystem<ecs::TransformSystem>();
	}

	void Application::Destroy()
	{
		//m_SFMLWindow->Destroy();
		m_EntityWorld.Destroy();
	}

	void Application::Update(const Saz::GameTime& gameTime)
	{
		m_EntityWorld.Update(gameTime);
		//m_ImGuiLog->Update();
	}

	void Application::DrawFrame()
	{
		uint32_t imageIndex;
		auto result = m_SwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return;
		}

		SAZ_ASSERT(result == VK_SUCCESS, "Failed to acquire swap chain image!");

		RecordCommandBuffer(imageIndex);
		bool success = m_SwapChain->submitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex) == VK_SUCCESS;
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_GLFWWindow->HasResized())
		{
			m_GLFWWindow->SetResized(false);
			RecreateSwapChain();
			return;
		}
		SAZ_ASSERT(success, "Failed to present swap chain image!");

		vkDeviceWaitIdle(m_Device->device());
	}

	void Application::Run(int argc, char* argv[])
	{
		Register();
		Init();

		sf::Clock clock;
		Saz::GameTime gameTime;

		while (true)
		{
			gameTime.m_Time = clock.restart();
			gameTime.m_DeltaTime = gameTime.m_Time.asSeconds();
			gameTime.m_TotalTime += gameTime.m_DeltaTime;
			gameTime.m_Frame++;

			/*m_SFMLWindow->Update(gameTime);
			if (m_SFMLWindow->ShouldClose())
				break;*/

			m_GLFWWindow->Update(gameTime);
			if (m_GLFWWindow->ShouldClose())
				break;

			Update(gameTime);
			DrawFrame();
		}
	}


	void Application::LoadModels()
	{
		DynamicArray<vulkan::Vertex> vertices
		{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		};

		m_Model = std::make_unique<vulkan::Model>(*m_Device, vertices);
	}

	void Application::RecreateSwapChain()
	{
		auto extent = m_GLFWWindow->GetSize();
		while(extent.x == 0 || extent.y == 0)
		{
			extent = m_GLFWWindow->GetSize();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_Device->device());
		m_SwapChain = nullptr;

		if (m_SwapChain == nullptr)
			m_SwapChain = std::make_unique<vulkan::SwapChain>(*m_Device, extent);
		else
		{
			m_SwapChain = std::make_unique<vulkan::SwapChain>(*m_Device, extent, std::move(m_SwapChain));
			if (m_SwapChain->imageCount() != m_CommandBuffers.size())
			{
				FreeCommandBuffers();
				CreateCommandBuffers();
			}
		}

		// if render pass is compatible do nothing else
		CreatePipeline();
	}

	void Application::RecordCommandBuffer(int imageIndex)
	{
		static int frame = 0;
		frame = (frame + 1) % 1000;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		bool beginSuccess = vkBeginCommandBuffer(m_CommandBuffers[imageIndex], &beginInfo) == VK_SUCCESS;
		SAZ_ASSERT(beginSuccess, "Failed to begin recording command buffer!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain->getRenderPass();
		renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { (uint32_t)m_SwapChain->getSwapChainExtent().x, (uint32_t)m_SwapChain->getSwapChainExtent().y };

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {0.01f,0.01f,0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_CommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().x);
		viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().y);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, { (uint32_t)m_SwapChain->getSwapChainExtent().x, (uint32_t)m_SwapChain->getSwapChainExtent().y } };
		vkCmdSetViewport(m_CommandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(m_CommandBuffers[imageIndex], 0, 1, &scissor);

		m_Pipeline->Bind(m_CommandBuffers[imageIndex]);
		m_Model->Bind(m_CommandBuffers[imageIndex]);
		

		for(int j = 0; j < 4; j++)
		{
			SimplePushConstantData push{};
			push.offset = { -0.5f + frame * 0.002f, -0.4f + j * 0.25f };
			push.color = { 0.0f, 0.0f, 0.2f + 0.2f * j };

			vkCmdPushConstants(
				m_CommandBuffers[imageIndex], 
				m_PipelineLayout, 
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0, sizeof(SimplePushConstantData),
				&push);
			m_Model->Draw(m_CommandBuffers[imageIndex]);
		}

		vkCmdEndRenderPass(m_CommandBuffers[imageIndex]);
		bool endSuccess = vkEndCommandBuffer(m_CommandBuffers[imageIndex]) == VK_SUCCESS;
		SAZ_ASSERT(endSuccess, "Failed to record command buffer!");
	}

	void Application::CreatePipelineLayout()
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

		bool success = vkCreatePipelineLayout(m_Device->device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) == VK_SUCCESS;
		SAZ_ASSERT(success, "Failed to create Pipeline Layout!");
	}

	void Application::CreatePipeline()
	{
		SAZ_ASSERT(m_SwapChain != nullptr, "Cannot create pipeline before swap chain!");
		SAZ_ASSERT(m_PipelineLayout != nullptr, "Cannot create pipeline before pipeline layout!");

		vulkan::PipelineConfig pipelineConfig{};
		vulkan::Pipeline::DefaultPipelineConfig(pipelineConfig);

		pipelineConfig.renderPass = m_SwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = m_PipelineLayout;
		m_Pipeline = std::make_unique<vulkan::Pipeline>(*m_Device, pipelineConfig);
	}

	void Application::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(m_SwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_Device->getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

		bool success = vkAllocateCommandBuffers(m_Device->device(), &allocInfo, m_CommandBuffers.data()) == VK_SUCCESS;
		SAZ_ASSERT(success, "Failed to allocate command buffers!");
	}

	void Application::FreeCommandBuffers()
	{
		vkFreeCommandBuffers(m_Device->device(), m_Device->getCommandPool(), (uint32_t)m_CommandBuffers.size(), m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}

}