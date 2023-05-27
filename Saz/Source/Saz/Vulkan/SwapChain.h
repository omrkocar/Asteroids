#pragma once

#include <Saz/Vulkan/Device.h>
#include <Core/Vector.h>

#include <Vulkan/vulkan.h>

namespace vulkan
{
	class SwapChain
	{
	public:
		SwapChain(Device& device, Vector2Int extent);
		~SwapChain();

	private:
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes);
		Vector2Int ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		Device& m_Device;
		Vector2Int m_Extent;

		VkSwapchainKHR m_SwapChain;
		VkFormat m_SwapChainImageFormat;

		VkRenderPass m_RenderPass = VK_NULL_HANDLE;

		DynamicArray<VkImage> m_SwapChainImages;
		DynamicArray<VkImageView> m_SwapChainImageViews;
	};
}