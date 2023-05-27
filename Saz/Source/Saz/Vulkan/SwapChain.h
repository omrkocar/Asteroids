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
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes);
		Vector2Int ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		VkSwapchainKHR m_SwapChain;
		DynamicArray<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;

	private:
		Device& m_Device;
		Vector2Int m_Extent;
	};
}