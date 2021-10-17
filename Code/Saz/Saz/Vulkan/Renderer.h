#pragma once

#include <Vulkan/vulkan.h>

namespace vulkan {
	class Renderer {
	public:
		Renderer(glfw::Window& window, vulkan::Device& device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		VkRenderPass HetSwapChainRenderPass() const;
		bool IsFrameInProgress() const { return isFrameStarted; }

		VkCommandBuffer HetCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

		VkCommandBuffer getCurrentCommandBuffer() const;

		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecreateSwapChain();

		glfw::Window& m_Window;
		vulkan::Device& m_Device;
		std::unique_ptr<SwapChain> m_SwapChain;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		uint32_t m_ImageIndex = 0;
		uint32_t m_FrameIndex = 0;
		int m_CurrentFrameIndex{ 0 };
		bool m_IsFrameStarted{ false };
	};
}  // namespace lve