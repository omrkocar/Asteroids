#include "SazPCH.h"
#include "Device.h"

#include <GLFW/glfw3.h>

namespace
{
	static const bool enableValidationLayers = SAZ_DEBUG;

	bool CheckValidationLayerSupport(const DynamicArray<const char*>& validationLayers)
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		DynamicArray<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;

	}

	DynamicArray<const char*> GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		DynamicArray<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		SAZ_CORE_TRACE("validation layer: {0}", pCallbackData->pMessage);

		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		else
			return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}
}

namespace vulkan
{
	Device::Device()
	{
		CreateInstance();
		SetupDebugMessenger();
		PickPhysicalDevice();
		CreateLogicalDevice();
	}

	Device::~Device()
	{
		if (enableValidationLayers)
			DestroyDebugUtilsMessengerEXT(m_VkInstance, m_DebugMessenger, nullptr);
		vkDestroyInstance(m_VkInstance, nullptr);
		vkDestroyDevice(m_Device, nullptr);
	}

	void Device::CreateInstance()
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Saz Engine";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;


		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &m_VkInstance) != VK_SUCCESS)
		{
			SAZ_CORE_ASSERT(false, "Vulkan instance failed to create!");
		}
	}

	void Device::PopulateDebugMessengerCreateInfo(
		VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
		createInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr;  // Optional
	}

	void Device::SetupDebugMessenger() {
		if (!enableValidationLayers) return;
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);
		bool success = CreateDebugUtilsMessengerEXT(m_VkInstance, &createInfo, nullptr, &m_DebugMessenger) == VK_SUCCESS;
		SAZ_CORE_ASSERT(success, "failed to set up debug messenger!");
	}

	void Device::PickPhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, nullptr);

		if (deviceCount == 0)
			SAZ_CORE_ASSERT(false, "No device found with Vulkan support!");

		DynamicArray<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, devices.data());

		for (const auto& device : devices)
		{
			if (IsDeviceSuitable(device))
			{
				m_PhysicalDevice = device;
				break;
			}
		}

		if (m_PhysicalDevice == VK_NULL_HANDLE)
			SAZ_CORE_ASSERT(false, "Failed to find a suitable GPU!");
	}

	bool Device::IsDeviceSuitable(VkPhysicalDevice device)
	{
		auto indices = FindQueueFamilies(device);
		return indices.IsComplete();
	}

	QueueFamilyIndices Device::FindQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		DynamicArray<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				indices.graphicsFamily = i;

			if (indices.IsComplete())
				break;

			i++;
		}

		return indices;
	}

	void Device::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;

		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;

		if (vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to create logical device!");

		vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
	}

}