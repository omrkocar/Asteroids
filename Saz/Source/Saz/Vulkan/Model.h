#pragma once

#include "Core/DynamicArray.h"
#include "Core/Vector.h"

#include <Vulkan/vulkan.h>

namespace vulkan 
{ 
	class Device; 
}

namespace vulkan
{
	struct Vertex {
		Vector2 pos;
		Vector3 color;

		static VkVertexInputBindingDescription GetBindingDescription();
		static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescription();
	};

	class Model
	{
	public:
		Model(Device& device, const DynamicArray<Vertex>& vertices);
		~Model();

		void CreateVertexBuffer(const DynamicArray<Vertex>& vertices);
		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		Device& m_Device;
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;

		uint32_t m_VertexCount = 0;
	};
}