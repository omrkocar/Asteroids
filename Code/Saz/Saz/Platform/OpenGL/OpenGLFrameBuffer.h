#pragma once

#include "Saz/Rendering/Framebuffer.h"

namespace Saz
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual const FrameBufferSpecification& GetSpecification() const { return m_Specification; }
		virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void Invalidate() override;

		virtual void Bind() override;
		virtual void Unbind() override;
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0;
		uint32_t m_DepthAttachment = 0;
		FrameBufferSpecification m_Specification;
	};
}