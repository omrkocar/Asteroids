#include "SazPCH.h"
#include "Renderer.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include <memory>
#include "Saz/Core/WindowBase.h"

namespace vulkan
{
	
	Renderer::Renderer(Saz::WindowsWindow& window, vulkan::Device& device)
		: m_Window(window)
		, m_Device(device)
	{
		CreateCommandBuffers();

		m_SwapChain = std::make_unique<vulkan::SwapChain>(m_Device, m_Window.GetSize());

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
		vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = { 1920, 1080 };
		vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);

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

	void Renderer::DrawFrame()
	{
		auto device = m_Device.device();
		vkWaitForFences(device, 1, &m_InFlightFence, VK_TRUE, UINT64_MAX);

		vkResetFences(device, 1, &m_InFlightFence);

		uint32_t imageIndex;
		vkAcquireNextImageKHR(device, m_SwapChain->m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		vkResetCommandBuffer(m_CommandBuffer, 0);
		RecordCommandBuffer(m_CommandBuffer, imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(m_Device.m_GraphicsQueue, 1, &submitInfo, m_InFlightFence) != VK_SUCCESS)
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
	}

	void Renderer::CreateCommandBuffers()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_Device.GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, &m_CommandBuffer) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to allocate command buffers!");
	}

	Renderer::~Renderer()
	{
		auto device = m_Device.device();
		vkDestroySemaphore(device, m_ImageAvailableSemaphore, nullptr);
		vkDestroySemaphore(device, m_RenderFinishedSemaphore, nullptr);
		vkDestroyFence(device, m_InFlightFence, nullptr);
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
		VkRect2D scissor{ {0, 0}, { m_SwapChain->m_Extent.x, m_SwapChain->m_Extent.y } };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->m_GraphicsPipeline);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to record command buffer!");
	}

	void Renderer::CreateSyncObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		auto device = m_Device.device();
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &m_InFlightFence) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "failed to create semaphores!");
	}

}