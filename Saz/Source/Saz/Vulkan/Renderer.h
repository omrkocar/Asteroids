#pragma once

#include <Saz/Core/WindowsWindow.h>
#include <Saz/Vulkan/Device.h>

#include <Vulkan/vulkan.h>

namespace vulkan 
{ 
	class Pipeline;
	class SwapChain;
}

namespace vulkan
{
	class Renderer final
	{
	public:
		static constexpr int s_MaxFramesInFlight = 2;

		Renderer(Saz::WindowsWindow& window, Device& device);
		~Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		VkCommandBuffer BeginFrame();
		void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);

		void EndFrame();

	private:
		void RecreateSwapChain();
		void CreateCommandBuffers();
		void CreateSyncObjects();
		VkCommandBuffer GetCurrentCommandBuffer() const;

	private:
		Saz::WindowsWindow& m_Window;
		Device& m_Device;

		std::unique_ptr<vulkan::SwapChain> m_SwapChain = nullptr;
		std::unique_ptr<vulkan::Pipeline> m_Pipeline = nullptr;

		DynamicArray<VkSemaphore> m_ImageAvailableSemaphores;
		DynamicArray<VkSemaphore> m_RenderFinishedSemaphores;
		DynamicArray<VkFence> m_InFlightFences;

		DynamicArray<VkCommandBuffer> m_CommandBuffers;

		uint32_t m_CurrentFrame = 0;
		uint32_t m_ImageIndex = 0;
	};
}