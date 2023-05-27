#pragma once

#include <Core/DynamicArray.h>

#include <Vulkan/vulkan.h>

#include <optional>

namespace Saz
{
	class WindowsWindow;
}

namespace vulkan
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		DynamicArray<VkSurfaceFormatKHR> formats;
		DynamicArray<VkPresentModeKHR> presentModes;
	};

	class Device
	{
	public:
		Device(Saz::WindowsWindow& window);
		~Device();

	public:
		SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(m_PhysicalDevice); }
		QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(m_PhysicalDevice); }
		VkSurfaceKHR GetSurface() { return m_Surface; }
		VkDevice GetDevice() { return m_Device; }

	private:
		void CreateInstance();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();

		void PickPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

		void CreateLogicalDevice();
		void CreateSurface();

		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const DynamicArray<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const DynamicArray<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void CreateSwapChain();

	private:
		// Implicitly destroyed when VkInstance is
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		VkInstance m_VkInstance;
		VkSurfaceKHR m_Surface;

		VkDebugUtilsMessengerEXT m_DebugMessenger;

		VkPhysicalDeviceProperties m_DeviceProperties;

		const DynamicArray<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
		const DynamicArray<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	private:
		Saz::WindowsWindow& m_Window;
	};
}