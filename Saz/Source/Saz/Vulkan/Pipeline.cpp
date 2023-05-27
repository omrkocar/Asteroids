#include "SazPCH.h"
#include "Pipeline.h"

#include <fstream>

namespace
{
	VkShaderModule CreateShaderModule(const VkDevice device, const DynamicArray<char>& code)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to create shader module!");

		return shaderModule;
	}
}

namespace vulkan
{
	
	Pipeline::Pipeline(Device& device, const PipelineConfig& config)
		: m_Device(device)
	{
		CreateGraphicsPipeline(device.device(), config);
	}

	Pipeline::~Pipeline()
	{
		vkDestroyPipeline(m_Device.device(), m_GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
	}

	void Pipeline::DefaultPipelineConfig(PipelineConfig& config)
	{
		config.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		config.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		config.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		config.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		config.rasterizationInfo.depthClampEnable = VK_FALSE;
		config.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		config.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		config.rasterizationInfo.lineWidth = 1.0f;
		config.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		config.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		config.rasterizationInfo.depthBiasEnable = VK_FALSE;
		config.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
		config.rasterizationInfo.depthBiasClamp = 0.0f; // Optional
		config.rasterizationInfo.depthBiasSlopeFactor = 0.0f; // Optional

		config.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		config.multisampleInfo.sampleShadingEnable = VK_FALSE;
		config.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		config.multisampleInfo.minSampleShading = 1.0f; // Optional
		config.multisampleInfo.pSampleMask = nullptr; // Optional
		config.multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
		config.multisampleInfo.alphaToOneEnable = VK_FALSE; // Optional

		config.colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		config.colorBlendAttachment.blendEnable = VK_FALSE;
		config.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		config.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		config.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		config.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		config.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		config.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		config.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		config.colorBlendInfo.logicOpEnable = VK_FALSE;
		config.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
		config.colorBlendInfo.attachmentCount = 1;
		config.colorBlendInfo.pAttachments = &config.colorBlendAttachment;
		config.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
		config.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
		config.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
		config.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

		config.dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		config.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		config.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(config.dynamicStateEnables.size());
		config.dynamicStateInfo.pDynamicStates = config.dynamicStateEnables.data();
	}

	void Pipeline::CreateGraphicsPipeline(VkDevice device, const PipelineConfig& config)
	{
		auto vertShaderCode = ReadFile("../Data/SimpleShader.vert.spv");
		auto fragShaderCode = ReadFile("../Data/SimpleShader.frag.spv");

		VkShaderModule vertShaderModule = CreateShaderModule(device, vertShaderCode);
		VkShaderModule fragShaderModule = CreateShaderModule(device, fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to create pipeline layout!");

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;

		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &config.inputAssemblyInfo;
		pipelineInfo.pViewportState = &config.viewportInfo;
		pipelineInfo.pRasterizationState = &config.rasterizationInfo;
		pipelineInfo.pMultisampleState = &config.multisampleInfo;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &config.colorBlendInfo;
		pipelineInfo.pDynamicState = &config.dynamicStateInfo;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = config.renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		auto a = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline);
		if (a != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to create graphics pipeline!");

		vkDestroyShaderModule(device, fragShaderModule, nullptr);
		vkDestroyShaderModule(device, vertShaderModule, nullptr);
	}

	DynamicArray<char> Pipeline::ReadFile(const String& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			SAZ_CORE_ASSERT(false, "Failed to open file!");

		size_t fileSize = (size_t)file.tellg();
		DynamicArray<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

}