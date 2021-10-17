#include "SazPCH.h"
#include "Saz/Vulkan/Renderer.h"

//#include "Saz/Vulkan/Geometry.h"
#include "Saz/Vulkan/Model.h"
#include "Saz/Vulkan/Pipeline.h"
#include "Saz/Vulkan/SwapChain.h"
#include "Saz/Vulkan/Device.h"

#include "Saz/GLFW/Window.h"

#include <GLFW/glfw3.h>
#include <Vulkan/vulkan.h>

namespace vulkan {

	Renderer::Renderer(glfw::Window& window, vulkan::Device& device)
		: m_Window{ window }, m_Device{ device } 
	{
		RecreateSwapChain();
		CreateCommandBuffers();
	}

	Renderer::~Renderer() 
	{ 
		FreeCommandBuffers(); 
	}

	void Renderer::RecreateSwapChain() {
		auto extent = m_Window.GetSize();
		while (extent.x == 0 || extent.y == 0) {
			extent = m_Window.GetSize();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(m_Device.device());

		if (m_SwapChain == nullptr) {
			m_SwapChain = std::make_unique<SwapChain>(m_Device, extent);
		}
		else {
			std::shared_ptr<SwapChain> oldSwapChain = std::move(m_SwapChain);
			m_SwapChain = std::make_unique<SwapChain>(m_Device, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*m_SwapChain.get())) {
				throw std::runtime_error("Swap chain image(or depth) format has changed!");
			}
		}
	}

	void Renderer::CreateCommandBuffers() {
		m_CommandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_Device.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

		if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void Renderer::FreeCommandBuffers() {
		vkFreeCommandBuffers(
			m_Device.device(),
			m_Device.getCommandPool(),
			static_cast<uint32_t>(m_CommandBuffers.size()),
			m_CommandBuffers.data());
		m_CommandBuffers.clear();
	}

	VkCommandBuffer Renderer::GetCurrentCommandBuffer() const
	{
		SAZ_ASSERT(m_IsFrameStarted, "Cannot get command buffer when frame not in progress");
		return m_CommandBuffers[m_CurrentFrameIndex];
	}

	VkCommandBuffer Renderer::BeginFrame() {
		SAZ_ASSERT(!m_IsFrameStarted, "Can't call beginFrame while already in progress");

		auto result = m_SwapChain->acquireNextImage(&m_ImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			RecreateSwapChain();
			return nullptr;
		}

		SAZ_ASSERT(result == VK_SUCCESS, "failed to acquire swap chain image!");

		m_IsFrameStarted = true;

		auto commandBuffer = GetCurrentCommandBuffer();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		bool success = vkBeginCommandBuffer(commandBuffer, &beginInfo) == VK_SUCCESS;
		SAZ_ASSERT(success, "failed to begin recording command buffer!");
		return commandBuffer;
	}

	void Renderer::EndFrame() {
		SAZ_ASSERT(m_IsFrameStarted, "Can't call endFrame while frame is not in progress");
		auto commandBuffer = GetCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}

		auto result = m_SwapChain->submitCommandBuffers(&commandBuffer, &m_ImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
			m_Window().HasResized()) {
			m_Window.SetResized(false);
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		m_IsFrameStarted = false;
		m_FrameIndex = (m_FrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		SAZ_ASSERT(m_IsFrameStarted, "Can't call beginSwapChainRenderPass if frame is not in progress");
		SAZ_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Can't begin render pass on command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain->getRenderPass();
		renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(m_ImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, m_SwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
		SAZ_ASSERT(m_IsFrameStarted, "Can't call endSwapChainRenderPass if frame is not in progress");
		SAZ_ASSERT(commandBuffer == GetCurrentCommandBuffer(), "Can't end render pass on command buffer from a different frame");
		vkCmdEndRenderPass(commandBuffer);
	}

	VkRenderPass LveRenderer::HetSwapChainRenderPass() const
	{
		return m_SwapChain->getRenderPass();
	}


}  // namespace vulkan

