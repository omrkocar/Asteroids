#pragma once

#include <Vulkan/vulkan.h>


namespace vulkan
{
	class Device;
	class Buffer;

	struct Vertex
	{
		vec3 position{};
		vec3 color{};
		vec3 normal{};
		vec2 uv{};

		static DynamicArray<VkVertexInputBindingDescription> GetBindingDescriptions();
		static DynamicArray<VkVertexInputAttributeDescription> GetAttributeDescriptions();

		bool operator==(const Vertex& other) const {
			return position == other.position && color == other.color && uv == other.uv;
		}
	};

	class Model
	{
	public:
		struct Builder
		{
			DynamicArray<Vertex> vertices{};
			DynamicArray<uint32_t> indices{};

			void LoadModel(const String& filepath);
		};

		Model(vulkan::Device& device, const vulkan::Model::Builder& builder);
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

		static std::unique_ptr<Model> CreateModelFromFile(vulkan::Device& device, const String& filepath);
		static void CreateTextureImage();

	private:
		void CreateVertexBuffers(const DynamicArray<Vertex>& vertices);
		void CreateIndexBuffers(const DynamicArray<uint32_t>& indices);

	private:
		vulkan::Device& m_Device;

		std::unique_ptr<vulkan::Buffer> m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;
		uint32_t m_VertexCount;

		bool m_HasIndexBuffer = false;
		std::unique_ptr<vulkan::Buffer> m_IndexBuffer;
		VkDeviceMemory m_IndexBufferMemory;
		uint32_t m_IndexCount;
	};
}