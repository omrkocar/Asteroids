#pragma once

#include <Vulkan/vulkan.h>


namespace vulkan
{
	class Device;

	struct Vertex
	{
		vec2 position;
		vec3 color;

		static DynamicArray<VkVertexInputBindingDescription> GetBindingDescriptions();
		static DynamicArray<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	};

	class Model
	{
	public:
		Model(vulkan::Device& device, const DynamicArray<Vertex>& vertices);
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		void CreateVertexBuffers(const DynamicArray<Vertex>& vertices);

	private:
		vulkan::Device& m_Device;
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;
		uint32_t m_VertexCount;
	};
}