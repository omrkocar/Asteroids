#pragma once

#include "Saz/Rendering/Framebuffer.h"

namespace Saz
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void Invalidate() override;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void ClearColorAttachment(uint32_t attachmentIndex, int value) override;

		virtual const FrameBufferSpecification& GetSpecification() const { return m_Specification; }
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { SAZ_CORE_ASSERT(index < m_ColorAttachments.size(), ""); return m_ColorAttachments[index]; }

	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_Specification;

		DynamicArray<FrameBufferTextureSpecification> m_ColorAttachmentSpecs;
		FrameBufferTextureSpecification m_DepthAttachmentSpec;

		DynamicArray<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};
}