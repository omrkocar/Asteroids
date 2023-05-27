#include "SazPCH.h"
#include "Renderer.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include <memory>
#include "Saz/Core/WindowBase.h"

namespace vulkan
{
	
	Renderer::Renderer(Saz::WindowsWindow& window, vulkan::Device& device)
		: m_Window(window)
		, m_Device(device)
	{
		CreateCommandBuffers();

		m_SwapChain = std::make_unique<vulkan::SwapChain>(m_Device, m_Window.GetSize());

		vulkan::PipelineConfig pipelineConfig{};
		vulkan::Pipeline::DefaultPipelineConfig(pipelineConfig);
		pipelineConfig.renderPass = m_SwapChain->m_RenderPass;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)1920;
		viewport.height = (float)1080;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = { 1920, 1080 };
		vkCmdSetScissor(m_CommandBuffer, 0, 1, &scissor);

		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = &viewport;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = &scissor;

		pipelineConfig.viewportInfo = viewportInfo;

		m_Pipeline = std::make_unique<vulkan::Pipeline>(m_Device, pipelineConfig);
	}

	void Renderer::CreateCommandBuffers()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = m_Device.GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(m_Device.GetDevice(), &allocInfo, &m_CommandBuffer) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to allocate command buffers!");
	}

	Renderer::~Renderer()
	{

	}

	void Renderer::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		// this also implicitly resets the previously recorded buffer if any.
		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to begin recording command buffer!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain->m_RenderPass;
		renderPassInfo.framebuffer = m_SwapChain->GetFrameBuffer(imageIndex);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { (uint32_t)m_SwapChain->m_Extent.x, (uint32_t)m_SwapChain->m_Extent.y };

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->m_GraphicsPipeline);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			SAZ_CORE_ASSERT(false, "Failed to record command buffer!");
	}

}