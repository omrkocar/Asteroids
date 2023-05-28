#pragma once

#include <Saz/Vulkan/Device.h>

#include <Vulkan/vulkan.h>

namespace vulkan::Utils
{
	void CreateBuffer(
		VkDevice logicalDevice,
		VkPhysicalDevice physicalDevice,
		VkDeviceSize size,
		VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties,
		VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);

	uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void CopyBuffer(
		VkCommandPool commandPool,
		VkDevice logicalDevice,
		VkQueue graphicsQueue, 
		VkBuffer srcBuffer, 
		VkBuffer dstBuffer, 
		VkDeviceSize size);
}