#include "SazPCH.h"
#include "Model.h"

#include "Saz/Vulkan/Device.h"
#include "Saz/Vulkan/Utils.h"

#include <GLFW/glfw3.h>

namespace vulkan
{
	Model::Model(Device& device, const DynamicArray<Vertex>& vertices)
		: m_Device(device)
		, m_VertexCount(static_cast<uint32_t>(vertices.size()))
	{
		CreateVertexBuffer(vertices);
	}

	Model::~Model()
	{
		vkDestroyBuffer(m_Device.device(), m_VertexBuffer, nullptr);
		vkFreeMemory(m_Device.device(), m_VertexBufferMemory, nullptr);
	}

	void Model::CreateVertexBuffer(const DynamicArray<Vertex>& vertices)
	{
		const VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		Utils::CreateBuffer(
			m_Device.device(),
			m_Device.m_PhysicalDevice,
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, 
			stagingBufferMemory);

		void* data;
		vkMapMemory(m_Device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), bufferSize);
		vkUnmapMemory(m_Device.device(), stagingBufferMemory);

		Utils::CreateBuffer(
			m_Device.device(),
			m_Device.m_PhysicalDevice, 
			bufferSize, 
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
			m_VertexBuffer, 
			m_VertexBufferMemory);

		Utils::CopyBuffer(m_Device.m_CommandPool, m_Device.device(), m_Device.m_GraphicsQueue, stagingBuffer, m_VertexBuffer, bufferSize);
		vkDestroyBuffer(m_Device.device(), stagingBuffer, nullptr);
		vkFreeMemory(m_Device.device(), stagingBufferMemory, nullptr);
	}

	void Model::Bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer vertexBuffers[] = { m_VertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	}

	void Model::Draw(VkCommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
	}

	VkVertexInputBindingDescription Vertex::GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		// VK_VERTEX_INPUT_RATE_INSTANCE for instanced rendering
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::array<VkVertexInputAttributeDescription, 2> Vertex::GetAttributeDescription()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		return attributeDescriptions;
	}

}