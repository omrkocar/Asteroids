#include "SazPCH.h"
#include "Renderer.h"

#include "Saz/Core/WindowBase.h"
#include "Saz/Vulkan/Model.h"
#include "Saz/Vulkan/Pipeline.h"
#include "Saz/Vulkan/SwapChain.h"

#include <GLFW/glfw3.h>

namespace vulkan
{
	
	Renderer::Renderer(Saz::WindowsWindow& window, vulkan::Device& device)
		: m_Window(window)
		, m_Device(device)
	{
		// TODO OK: Remove Model
		const std::vector<Vertex> vertices = {
			{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		m_Model = std::make_unique<Model>(m_Device, vertices);

		RecreateSwapChain();
		CreateCommandBuffers();

		vulkan::PipelineConfig pipelineConfig{};
		vulkan::Pipeline::DefaultPipelineConfig(pipelineConfig);
		pipelineConfig.renderPass = m_SwapChain->m_RenderPass;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)1920;
		viewport.height = (float)1080;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_CommandBuffers[m_CurrentFrame], 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = { 1920, 1080 };
		vkCmdSetScissor(m_CommandBuffers[m_CurrentFrame], 0, 1, &scissor);

		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = &viewport;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = &scissor;

		pipelineConfig.viewportInfo = viewportInfo;

		m_Pipeline = std::make_unique<vulkan::Pipeline>(m_Device, pipelineConfig);

		CreateSyncObjects();
	}

	Renderer::~Renderer()
	{
		for (size_t i = 0; i < s_MaxFramesInFlight; i++)
		{
			auto device = m_Device.device();
			vkDestroySemaphore(device, m_ImageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(device, m_RenderFinishedSemaphores[i], nullptr);
			vkDestroyFence(device, m_InFlightFences[i], nullptr);
		}
	}


	void Renderer::DrawFrame()
	{
		auto device = m_Device.device();
		vkWaitForFences(device, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		auto result = vkAcquireNextImageKHR(
			device, 
			m_SwapChain->m_SwapChain, 
			UINT64_MAX, 
			m_ImageAvailableSemaphores[m_CurrentFrame], 
			VK_NULL_HANDLE, 
			&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window.HasResized())
		{
			RecreateSwapChain();
			m_Window.SetResized(false);
			return;
		}

		SAZ_CORE_ASSERT(result == VK_SUCCESS, "Failed to acquire swap chain image!");

		vkResetFences(device, 1, &m_InFlightFences[m_CurrentFrame]);

		vkResetCommandBuffer(m_CommandBuffers[m_CurrentFrame], 0);
		RecordCommandBuffer(m_CommandBuffers[m_CurrentFrame], imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentFrame];

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(m_Device.m_GraphicsQueue, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to submit draw command buffer!");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_SwapChain->m_SwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(m_Device.m_PresentQueue, &presentInfo);

		m_CurrentFrame = (m_CurrentFrame + 1) % s_MaxFramesInFlight;
	}

	void Renderer::RecreateSwapChain()
	{
		Vector2Int extent = m_Window.GetSize();
		while (extent.x == 0 || extent.y == 0) {
			extent = m_Window.GetSize();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_Device.device());

		if (m_SwapChain == nullptr)
			m_SwapChain = std::make_unique<vulkan::SwapChain>(m_Device, m_Window.GetSize());
		else
		{
			std::shared_ptr<SwapChain> oldSwapChain = std::move(m_SwapChain);
			m_SwapChain = std::make_unique<vulkan::SwapChain>(m_Device, m_Window.GetSize(), oldSwapChain);
		}
	}

	void Renderer::CreateCommandBuffers()
	{
		m_CommandBuffers.resize(s_MaxFramesInFlight);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_Device.GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

		if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to allocate command buffers!");
	}

	void Renderer::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		// this also implicitly resets the previously recorded buffer if any.
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to begin recording command buffer!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain->m_RenderPass;
		renderPassInfo.framebuffer = m_SwapChain->GetFrameBuffer(imageIndex);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { (uint32_t)m_SwapChain->m_Extent.x, (uint32_t)m_SwapChain->m_Extent.y };

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_SwapChain->m_Extent.x);
		viewport.height = static_cast<float>(m_SwapChain->m_Extent.y);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, { static_cast<uint32_t>(m_SwapChain->m_Extent.x), static_cast<uint32_t>(m_SwapChain->m_Extent.y) } };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->m_GraphicsPipeline);

		m_Model->Bind(commandBuffer);
		m_Model->Draw(commandBuffer);

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to record command buffer!");
	}

	void Renderer::CreateSyncObjects()
	{
		int maxFramesInFlight = s_MaxFramesInFlight;
		m_ImageAvailableSemaphores.resize(maxFramesInFlight);
		m_RenderFinishedSemaphores.resize(maxFramesInFlight);
		m_InFlightFences.resize(maxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (int i = 0; i < maxFramesInFlight; i++)
		{
			auto device = m_Device.device();
			if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
				SAZ_CORE_ASSERT(false, "failed to create semaphores!");
		}
	}

}