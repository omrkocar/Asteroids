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
	
	Pipeline::Pipeline(Device& device)
		: m_Device(device)
	{
		CreateGraphicsPipeline();
	}

	Pipeline::~Pipeline()
	{

	}

	void Pipeline::CreateGraphicsPipeline()
	{
		auto vertShaderCode = ReadFile("../Data/SimpleShader.vert.spv");
		auto fragShaderCode = ReadFile("../Data/SimpleShader.frag.spv");

		VkDevice device = m_Device.GetDevice();
		VkShaderModule vertShaderModule = CreateShaderModule(device, vertShaderCode);
		VkShaderModule fragShaderModule = CreateShaderModule(device, fragShaderCode);

		vkDestroyShaderModule(device, fragShaderModule, nullptr);
		vkDestroyShaderModule(device, vertShaderModule, nullptr);

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