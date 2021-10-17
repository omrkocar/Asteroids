#pragma once

#include <Vulkan/vulkan.h>

namespace Saz::glfw
{
	class Window;
}

namespace vulkan
{
	class Device;
	class SwapChain;
}

namespace vulkan 
{
	class SAZ_API Renderer final
	{
	public:
		Renderer(Saz::glfw::Window& window, vulkan::Device& device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		VkRenderPass GetSwapChainRenderPass() const;
		bool IsFrameInProgress() const { return m_IsFrameStarted; }

		VkCommandBuffer GetCurrentCommandBuffer() const;

		int GetFrameIndex() const;

		VkCommandBuffer BeginFrame();
		void EndFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecreateSwapChain();

		Saz::glfw::Window& m_Window;
		vulkan::Device& m_Device;
		std::unique_ptr<vulkan::SwapChain> m_SwapChain;
		std::vector<VkCommandBuffer> m_CommandBuffers;

		uint32_t m_ImageIndex = 0;
		int m_FrameIndex{ 0 };
		bool m_IsFrameStarted{ false };
	};
}  // namespace lve