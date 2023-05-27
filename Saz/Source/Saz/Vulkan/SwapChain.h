#pragma once

#include <Saz/Vulkan/Device.h>
#include <Core/Vector.h>

#include <Vulkan/vulkan.h>

namespace vulkan
{
	class SwapChain
	{
		friend class Renderer;

	public:
		SwapChain(Device& device, Vector2Int extent);
		~SwapChain();


		VkRenderPass GetRenderPass() { return m_RenderPass; }
		VkFramebuffer GetFrameBuffer(int index) { return m_SwapChainFramebuffers[index]; }
		Vector2Int GetExtent() { return m_Extent; }

	private:
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateFrameBuffers();
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes);
		Vector2Int ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		Device& m_Device;
		Vector2Int m_Extent;

		VkSwapchainKHR m_SwapChain;
		VkFormat m_SwapChainImageFormat;

		VkRenderPass m_RenderPass = VK_NULL_HANDLE;

		DynamicArray<VkFramebuffer> m_SwapChainFramebuffers;
		DynamicArray<VkImage> m_SwapChainImages;
		DynamicArray<VkImageView> m_SwapChainImageViews;
	};
}