#pragma once

#include <Vulkan/vulkan.h>

namespace vulkan
{
	class Device
	{
	public:
		Device();
		~Device();

	private:
		void CreateInstance();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();

	private:
		VkInstance m_VkInstance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;

		const DynamicArray<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	};
}