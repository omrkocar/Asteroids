#pragma once

#include <Saz/Vulkan/Device.h>
#include <Core/Vector.h>

#include <Vulkan/vulkan.h>
#include <Core/String.h>

namespace vulkan
{
	struct PipelineConfig
	{
		PipelineConfig() = default;
		PipelineConfig(const PipelineConfig&) = delete;
		PipelineConfig& operator=(const PipelineConfig&) = delete;

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		DynamicArray<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class Pipeline
	{
		friend class Renderer;

	public:
		Pipeline(Device& device, const PipelineConfig& config);
		~Pipeline();

		static void DefaultPipelineConfig(PipelineConfig& config);

	private:
		void CreateGraphicsPipeline(VkDevice device, const PipelineConfig& config);

		static DynamicArray<char> ReadFile(const String& filename);
	private:
		Device& m_Device;

		VkPipeline m_GraphicsPipeline;
		VkPipelineLayout m_PipelineLayout;

		DynamicArray<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
	};
}