#pragma once

#include <Core/DynamicArray.h>

#include "Saz/Core/WindowsWindow.h"
#include <Saz/Vulkan/Device.h>

#include <Vulkan/vulkan.h>

namespace vulkan { class SwapChain; }

namespace vulkan 
{ 
	class Pipeline; 
}

namespace Saz
{
	
}

namespace vulkan
{
	class Renderer final
	{
	public:
		Renderer(Saz::WindowsWindow& window, Device& device);
		~Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		void DrawFrame();

	private:
		void CreateCommandBuffers();
		void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void CreateSyncObjects();

	private:
		Saz::WindowsWindow& m_Window;
		Device& m_Device;

		std::unique_ptr<vulkan::SwapChain> m_SwapChain = nullptr;
		std::unique_ptr<vulkan::Pipeline> m_Pipeline = nullptr;

		VkSemaphore m_ImageAvailableSemaphore;
		VkSemaphore m_RenderFinishedSemaphore;
		VkFence m_InFlightFence;

		VkCommandBuffer m_CommandBuffer;
	};
}