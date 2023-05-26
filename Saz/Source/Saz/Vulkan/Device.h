#pragma once

#include <Vulkan/vulkan.h>

namespace vulkan
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;

		bool IsComplete()
		{
			return graphicsFamily.has_value();
		}
	};

	class Device
	{
	public:
		Device();
		~Device();

	private:
		void CreateInstance();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();
		void PickPhysicalDevice();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	private:
		// Implicitly destroyed when VkInstance is
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkInstance m_VkInstance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;

		VkPhysicalDeviceProperties m_DeviceProperties;
		VkPhysicalDeviceFeatures m_DeviceFeatures;

		const DynamicArray<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	};
}